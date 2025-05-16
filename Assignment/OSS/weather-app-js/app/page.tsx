"use client"

import { useState, useEffect } from "react"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import dynamic from "next/dynamic"

// 클라이언트 사이드에서만 로드되도록 컴포넌트 동적 임포트
const ApiKeyManager = dynamic(() => import("../api-key-manager"), { ssr: false })
const WeatherMap = dynamic(() => import("../weather-map"), { ssr: false })
const WeatherController = dynamic(() => import("../weather-controller"), { ssr: false })
const ApiMock = dynamic(() => import("../api-mock"), { ssr: false })
const ApiDiagnostics = dynamic(() => import("../api-diagnostics"), { ssr: false })

export default function Page() {
  const [useMock, setUseMock] = useState(false)
  const [isClient, setIsClient] = useState(false)

  // 컴포넌트 마운트 시 클라이언트 사이드 렌더링 확인
  useEffect(() => {
    setIsClient(true)

    // 로컬스토리지에서 모의 API 사용 여부 확인
    const savedUseMock = localStorage.getItem("useMockApi") === "true"
    setUseMock(savedUseMock)
  }, [])

  // 서버 사이드 렌더링 중에는 최소한의 UI만 표시
  if (!isClient) {
    return (
      <div className="container mx-auto py-10">
        <h1 className="text-3xl font-bold mb-6 text-center">날씨 앱</h1>
        <div className="flex justify-center">
          <div className="w-8 h-8 border-4 border-t-blue-500 border-b-blue-500 rounded-full animate-spin"></div>
        </div>
      </div>
    )
  }

  return (
    <div className="container mx-auto py-10">
      <h1 className="text-3xl font-bold mb-6 text-center">날씨 앱</h1>

      <div className="mb-6">
        <ApiKeyManager />
        <ApiMock />
        <ApiDiagnostics />
      </div>

      <Tabs defaultValue="weather" className="mt-6">
        <TabsList className="grid w-full grid-cols-2">
          <TabsTrigger value="weather">날씨 정보</TabsTrigger>
          <TabsTrigger value="map">지도 날씨</TabsTrigger>
        </TabsList>
        <TabsContent value="weather">
          <WeatherController />
        </TabsContent>
        <TabsContent value="map">
          <WeatherMap />
        </TabsContent>
      </Tabs>

      {useMock && (
        <div className="mt-8 p-4 bg-yellow-50 border border-yellow-200 rounded-md">
          <p className="text-yellow-800 text-sm">
            <strong>모의 API 모드:</strong> 현재 백엔드 서버 없이 모의 데이터를 사용하고 있습니다. 실제 API를 사용하려면
            API 모의 설정에서 모의 API 사용을 비활성화하세요.
          </p>
        </div>
      )}
    </div>
  )
}
