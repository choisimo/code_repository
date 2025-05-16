"use client"

import { useState, useEffect } from "react"
import { Button } from "@/components/ui/button"
import { Input } from "@/components/ui/input"
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/components/ui/card"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert"
import { AlertCircle, Check, Key } from "lucide-react"

export default function ApiKeyManager() {
  // API 키 상태 관리
  const [naverMapsKey, setNaverMapsKey] = useState("")
  const [weatherApiKey, setWeatherApiKey] = useState("")
  const [showSuccess, setShowSuccess] = useState(false)
  const [error, setError] = useState("")

  // 컴포넌트 마운트 시 로컬스토리지에서 API 키 로드
  useEffect(() => {
    const savedNaverKey = localStorage.getItem("naverMapsApiKey")
    const savedWeatherKey = localStorage.getItem("weatherApiKey")

    if (savedNaverKey) setNaverMapsKey(savedNaverKey)
    if (savedWeatherKey) setWeatherApiKey(savedWeatherKey)
  }, [])

  // API 키 저장 함수
  const saveApiKeys = () => {
    try {
      // 네이버 지도 API 키 저장
      if (naverMapsKey) {
        localStorage.setItem("naverMapsApiKey", naverMapsKey)
      }

      // 날씨 API 키 저장
      if (weatherApiKey) {
        localStorage.setItem("weatherApiKey", weatherApiKey)
      }

      // 성공 메시지 표시
      setShowSuccess(true)
      setError("")

      // 페이지 새로고침 (API 키 적용을 위해)
      if (naverMapsKey || weatherApiKey) {
        // 사용자에게 새로고침 알림
        const message = "API 키가 저장되었습니다. 변경사항을 적용하기 위해 페이지를 새로고침합니다."
        setShowSuccess(true)

        // 3초 후 페이지 새로고침
        setTimeout(() => {
          window.location.reload()
        }, 3000)
      }
    } catch (err) {
      setError("API 키 저장 중 오류가 발생했습니다. 브라우저 설정을 확인해주세요.")
    }
  }

  // API 키 삭제 함수
  const clearApiKeys = () => {
    try {
      localStorage.removeItem("naverMapsApiKey")
      localStorage.removeItem("weatherApiKey")
      setNaverMapsKey("")
      setWeatherApiKey("")
      setShowSuccess(true)
      setError("")

      setTimeout(() => {
        setShowSuccess(false)
      }, 3000)
    } catch (err) {
      setError("API 키 삭제 중 오류가 발생했습니다.")
    }
  }

  return (
    <Card className="w-full max-w-md mx-auto">
      <CardHeader>
        <CardTitle className="flex items-center gap-2">
          <Key className="h-5 w-5" />
          API 키 관리
        </CardTitle>
        <CardDescription>
          네이버 지도 API와 날씨 API 키를 설정하세요. 입력한 키는 브라우저에 안전하게 저장됩니다.
        </CardDescription>
      </CardHeader>
      <CardContent className="space-y-4">
        <Tabs defaultValue="naver">
          <TabsList className="grid w-full grid-cols-2">
            <TabsTrigger value="naver">네이버 지도 API</TabsTrigger>
            <TabsTrigger value="weather">날씨 API</TabsTrigger>
          </TabsList>

          <TabsContent value="naver" className="space-y-4 pt-4">
            <div className="space-y-2">
              <label htmlFor="naver-api-key" className="text-sm font-medium">
                네이버 지도 API 클라이언트 ID
              </label>
              <Input
                id="naver-api-key"
                value={naverMapsKey}
                onChange={(e) => setNaverMapsKey(e.target.value)}
                placeholder="네이버 클라이언트 ID를 입력하세요"
              />
              <p className="text-xs text-muted-foreground">
                네이버 클라우드 플랫폼에서 발급받은 Maps API 클라이언트 ID를 입력하세요.
              </p>
            </div>
          </TabsContent>

          <TabsContent value="weather" className="space-y-4 pt-4">
            <div className="space-y-2">
              <label htmlFor="weather-api-key" className="text-sm font-medium">
                공공데이터포털 날씨 API 키
              </label>
              <Input
                id="weather-api-key"
                value={weatherApiKey}
                onChange={(e) => setWeatherApiKey(e.target.value)}
                placeholder="공공데이터포털 API 키를 입력하세요"
              />
              <p className="text-xs text-muted-foreground">
                공공데이터포털에서 발급받은 기상청 API 서비스 키를 입력하세요.
              </p>
            </div>
          </TabsContent>
        </Tabs>

        {error && (
          <Alert variant="destructive">
            <AlertCircle className="h-4 w-4" />
            <AlertTitle>오류</AlertTitle>
            <AlertDescription>{error}</AlertDescription>
          </Alert>
        )}

        {showSuccess && (
          <Alert className="bg-green-50 text-green-800 border-green-200">
            <Check className="h-4 w-4 text-green-600" />
            <AlertTitle>성공</AlertTitle>
            <AlertDescription>API 키가 성공적으로 저장되었습니다.</AlertDescription>
          </Alert>
        )}
      </CardContent>
      <CardFooter className="flex justify-between">
        <Button variant="outline" onClick={clearApiKeys}>
          초기화
        </Button>
        <Button onClick={saveApiKeys}>저장</Button>
      </CardFooter>
    </Card>
  )
}
