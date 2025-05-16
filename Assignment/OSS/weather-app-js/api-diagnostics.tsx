"use client"

import { useState } from "react"
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/components/ui/card"
import { Button } from "@/components/ui/button"
import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert"
import { AlertCircle, CheckCircle, Loader2 } from "lucide-react"
import { Accordion, AccordionContent, AccordionItem, AccordionTrigger } from "@/components/ui/accordion"
import apiService from "./api-service-improved"

export default function ApiDiagnostics() {
  const [loading, setLoading] = useState(false)
  const [results, setResults] = useState<{
    connection?: { success: boolean; message: string; details?: string }
    cors?: { success: boolean; message: string }
  }>({})
  const [error, setError] = useState("")

  // API 진단 실행
  const runDiagnostics = async () => {
    setLoading(true)
    setError("")

    try {
      // API 연결 테스트
      const connectionResult = await apiService.testApiConnection()

      // CORS 테스트
      const corsResult = await apiService.testCorsConfiguration()

      // 결과 설정
      setResults({
        connection: connectionResult,
        cors: corsResult,
      })
    } catch (err) {
      setError(`진단 실행 중 오류가 발생했습니다: ${err.message}`)
    } finally {
      setLoading(false)
    }
  }

  return (
    <Card className="w-full max-w-md mx-auto mt-6">
      <CardHeader>
        <CardTitle>API 진단 도구</CardTitle>
        <CardDescription>API 연결 및 응답 문제를 진단하고 해결 방안을 제시합니다.</CardDescription>
      </CardHeader>
      <CardContent className="space-y-4">
        {error && (
          <Alert variant="destructive">
            <AlertCircle className="h-4 w-4" />
            <AlertTitle>오류</AlertTitle>
            <AlertDescription>{error}</AlertDescription>
          </Alert>
        )}

        {results.connection && (
          <div className="space-y-2">
            <div className="flex items-center gap-2">
              {results.connection.success ? (
                <CheckCircle className="h-5 w-5 text-green-500" />
              ) : (
                <AlertCircle className="h-5 w-5 text-red-500" />
              )}
              <h3 className="font-medium">API 연결 테스트</h3>
            </div>

            <Alert variant={results.connection.success ? "default" : "destructive"}>
              <AlertDescription>{results.connection.message}</AlertDescription>
            </Alert>

            {results.connection.details && (
              <Accordion type="single" collapsible>
                <AccordionItem value="details">
                  <AccordionTrigger>응답 상세 정보</AccordionTrigger>
                  <AccordionContent>
                    <pre className="bg-gray-100 p-2 rounded text-xs overflow-x-auto">{results.connection.details}</pre>
                  </AccordionContent>
                </AccordionItem>
              </Accordion>
            )}
          </div>
        )}

        {results.cors && (
          <div className="space-y-2">
            <div className="flex items-center gap-2">
              {results.cors.success ? (
                <CheckCircle className="h-5 w-5 text-green-500" />
              ) : (
                <AlertCircle className="h-5 w-5 text-red-500" />
              )}
              <h3 className="font-medium">CORS 설정 테스트</h3>
            </div>

            <Alert variant={results.cors.success ? "default" : "destructive"}>
              <AlertDescription>{results.cors.message}</AlertDescription>
            </Alert>
          </div>
        )}

        {!loading && Object.keys(results).length > 0 && !results.connection?.success && (
          <Alert>
            <AlertTitle>문제 해결 방안</AlertTitle>
            <AlertDescription className="space-y-2">
              <p>다음 사항을 확인해보세요:</p>
              <ul className="list-disc pl-5 space-y-1">
                <li>백엔드 서버가 실행 중인지 확인</li>
                <li>API 엔드포인트 URL이 올바른지 확인</li>
                <li>API 키가 올바르게 설정되었는지 확인</li>
                <li>서버의 CORS 설정이 클라이언트 도메인을 허용하는지 확인</li>
                <li>네트워크 연결 상태 확인</li>
              </ul>
              <p className="mt-2">문제가 지속되면 모의 API 모드를 활성화하여 백엔드 없이 앱을 테스트할 수 있습니다.</p>
            </AlertDescription>
          </Alert>
        )}
      </CardContent>
      <CardFooter>
        <Button onClick={runDiagnostics} disabled={loading} className="w-full">
          {loading && <Loader2 className="mr-2 h-4 w-4 animate-spin" />}
          {loading ? "진단 중..." : "API 진단 실행"}
        </Button>
      </CardFooter>
    </Card>
  )
}
