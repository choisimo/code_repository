#include <drogon/drogon.h>
#include "WebController.h"
#include "BackupConfigController.h"
#include "BackupJobController.h"
#include "BackupManager.h"

int main() {
    // 앱 구성
    drogon::app().setLogPath("./logs")
        .setLogLevel(trantor::Logger::kInfo)
        .addListener("0.0.0.0", 7070)
        .setThreadNum(16)
        .setDocumentRoot("./web")
        .setStaticFilesCacheTime(0);

    // SQLite 데이터베이스 설정
    drogon::app().createDbClient("sqlite3", "", 0, "", "", "backup_system.db", 10);

    auto backupManager = std::make_shared<BackupManager>("backup_system.db");
    auto webSocketController = std::make_shared<BackupWebSocketController>();

    // 컨트롤러 등록 (REST API)
    drogon::app().registerController(std::make_shared<BackupConfigController>());
    drogon::app().registerController(std::make_shared<BackupJobController>());
    drogon::app().registerController(std::make_shared<BackupStatusController>());

    // WebSocket 컨트롤러 등록
    drogon::app().registerController(std::make_shared<BackupWebSocketController>());

    std::cout << "Server running on 0.0.0.0:8080" << std::endl;
    drogon::app().run();
    return 0;
}
