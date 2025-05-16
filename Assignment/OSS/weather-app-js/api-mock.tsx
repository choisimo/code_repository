"use client"

import { useState } from "react"
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/components/ui/card"
import { Button } from "@/components/ui/button"
import { Input } from "@/components/ui/input"
import { Checkbox } from "@/components/ui/checkbox"
import { Label } from "@/components/ui/label"
import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert"
import { Check, Server } from "lucide-react"

export default function ApiMock() {
  const [useMock, setUseMock] = useState(() => {
    // 로컬스토리지에서 설정 불러오기
    const saved = localStorage.getItem("useMockApi")
    return saved === "true"
  })

  const [mockDelay, setMockDelay] = useState(() => {
    // 로컬스토리지에서 설정 불러오기
    return Number.parseInt(localStorage.getItem("mockApiDelay") || "500", 10)
  })

  const [showSuccess, setShowSuccess] = useState(false)

  // 설정 저장
  const saveMockSettings = () => {
    localStorage.setItem("useMockApi", useMock.toString())
    localStorage.setItem("mockApiDelay", mockDelay.toString())

    // 성공 메시지 표시
    setShowSuccess(true)
    setTimeout(() => setShowSuccess(false), 3000)

    // 페이지 새로고침
    setTimeout(() => window.location.reload(), 1000)
  }

  return (
    <Card className="w-full max-w-md mx-auto mt-6">
      <CardHeader>
        <CardTitle className="flex items-center gap-2">
          <Server className="h-5 w-5" />
          API 모의 설정
        </CardTitle>
        <CardDescription>백엔드 서버 없이 모의 데이터를 사용하여 앱을 테스트할 수 있습니다.</CardDescription>
      </CardHeader>
      <CardContent className="space-y-4">
        <div className="flex items-center space-x-2">
          <Checkbox id="use-mock" checked={useMock} onCheckedChange={(checked) => setUseMock(!!checked)} />
          <Label htmlFor="use-mock">모의 API 사용</Label>
        </div>

        <div className="space-y-2">
          <Label htmlFor="mock-delay">모의 응답 지연 시간 (ms)</Label>
          <Input
            id="mock-delay"
            type="number"
            min="0"
            max="5000"
            value={mockDelay}
            onChange={(e) => setMockDelay(Number.parseInt(e.target.value, 10))}
            disabled={!useMock}
          />
          <p className="text-xs text-muted-foreground">실제 API 호출을 시뮬레이션하기 위한 지연 시간입니다.</p>
        </div>

        {showSuccess && (
          <Alert className="bg-green-50 text-green-800 border-green-200">
            <Check className="h-4 w-4 text-green-600" />
            <AlertTitle>성공</AlertTitle>
            <AlertDescription>설정이 성공적으로 저장되었습니다.</AlertDescription>
          </Alert>
        )}
      </CardContent>
      <CardFooter>
        <Button onClick={saveMockSettings} className="w-full">
          설정 저장
        </Button>
      </CardFooter>
    </Card>
  )
}
