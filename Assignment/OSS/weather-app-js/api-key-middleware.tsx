.springframework.stereotype.Component
.springframework.web.servlet.HandlerInterceptor
.springframework.web.servlet.ModelAndView
.springframework.web.servlet.config.annotation.InterceptorRegistry
.springframework.web.servlet.config.annotation.WebMvcConfigurer
.springframework.beans.factory.annotation.Value
.springframework.context.annotation.Configuration
.servlet.http.HttpServletRequest
.servlet.http.HttpServletResponse
.util.Arrays
.util.List

@Component
public
class ApiKeyMiddleware implements HandlerInterceptor {
  @Value("${weather.api.key:}")
  private String
  configuredApiKey

  private static final
  List<String>
  EXCLUDED_PATHS = Arrays.asList("/api/health", "/api/version")

  @Override
  public boolean
  preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
  throws
  Exception;
  {
  String
  path = request.getRequestURI()

  // 제외 경로는 API 키 검증 없이 통과
  if (EXCLUDED_PATHS.stream()
  .
  anyMatch(path::startsWith)
  ) {
            return true
}

// API 키 파라미터 확인
String
apiKey = request.getParameter("apiKey")

// 클라이언트에서 전달한 API 키가 없는 경우, 서버에 설정된 API 키 사용
if (apiKey == null || apiKey.isEmpty()) {
  // 서버에 설정된 API 키가 없으면 오류
  if (configuredApiKey == null || configuredApiKey.isEmpty()) {
    response.setStatus(HttpServletResponse.SC_UNAUTHORIZED)
    response.getWriter().write('{"error":"API 키가 필요합니다."}')
    return false;
  }

  // 서버에 설정된 API 키 사용
  request.setAttribute("apiKey", configuredApiKey)
  return true;
}

// 클라이언트에서 전달한 API 키 사용
request.setAttribute("apiKey", apiKey)
return true;
}

@Override
public
void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView)
throws
Exception
{
  // 후처리 로직 (필요한 경우)
}

@Override
public
void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex)
throws
Exception
{
  // 완료 후 로직 (필요한 경우)
}
}

@Configuration
public
class WebMvcConfig implements WebMvcConfigurer {
  private final
  ApiKeyMiddleware
  apiKeyMiddleware

  public WebMvcConfig(ApiKeyMiddleware apiKeyMiddleware) {
        this.apiKeyMiddleware = apiKeyMiddleware;
    }

  @Override
  public void
  addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(apiKeyMiddleware)
                .addPathPatterns("/api/**");
    }
}
