//
// Created by nodove on 2025-04-12.
//
#pragma once
#include <drogon/WebSocketController.h>

class BackupWebSocketController : public drogon::WebSocketController<BackupWebSocketController> {
public:
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/ws/backup_status");
    WS_PATH_LIST_END

    virtual void handleNewConnection(const drogon::HttpRequestPtr &req,
                                    const drogon::WebSocketConnectionPtr& wsConnPtr) override;

    virtual void handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
                                 std::string &&message,
                                 const drogon::WebSocketMessageType &type) override;

    virtual void handleConnectionClosed(const drogon::WebSocketConnectionPtr& wsConnPtr) override;

    // 모든 연결된 클라이언트에게 알림 전송
    static void notifyAll(const std::string& event, const Json::Value& data);

    // 특정 작업 상태 변경 알림
    static void notifyJobUpdate(int job_id, const std::string& status, const Json::Value& stats);

private:
    static std::mutex connectionsMutex_;
    static std::unordered_set<drogon::WebSocketConnectionPtr> connections_;
};

// static 멤버 초기화
std::mutex BackupWebSocketController::connectionsMutex_;
std::unordered_set<drogon::WebSocketConnectionPtr> BackupWebSocketController::connections_;

// 구현
void BackupWebSocketController::handleNewConnection(const drogon::HttpRequestPtr &req,
                                                   const drogon::WebSocketConnectionPtr& wsConnPtr) {
    std::lock_guard<std::mutex> lock(connectionsMutex_);
    connections_.insert(wsConnPtr);

    // 현재 활성 백업 작업 정보 전송
    auto dbClient = drogon::app().getDbClient();
    dbClient->execSqlAsync(
        "SELECT id, config_id, status, start_time FROM backup_jobs WHERE status = 'running'",
        [wsConnPtr](const drogon::orm::Result& result) {
            Json::Value activeJobs(Json::arrayValue);
            for (const auto& row : result) {
                Json::Value job;
                job["id"] = row["id"].as<int>();
                job["config_id"] = row["config_id"].as<int>();
                job["status"] = row["status"].as<std::string>();
                job["start_time"] = row["start_time"].as<std::string>();
                activeJobs.append(job);
            }

            Json::Value response;
            response["event"] = "init";
            response["data"]["active_jobs"] = activeJobs;

            wsConnPtr->send(drogon::utils::toJson(response));
        },
        [wsConnPtr](const drogon::orm::DrogonDbException& e) {
            Json::Value response;
            response["event"] = "error";
            response["data"]["message"] = e.base().what();
            wsConnPtr->send(drogon::utils::toJson(response));
        }
    );
}

void BackupWebSocketController::handleConnectionClosed(const drogon::WebSocketConnectionPtr& wsConnPtr) {
    std::lock_guard<std::mutex> lock(connectionsMutex_);
    connections_.erase(wsConnPtr);
}

void BackupWebSocketController::notifyAll(const std::string& event, const Json::Value& data) {
    Json::Value message;
    message["event"] = event;
    message["data"] = data;

    std::string jsonStr = drogon::utils::toJson(message);

    std::lock_guard<std::mutex> lock(connectionsMutex_);
    for (const auto& conn : connections_) {
        if (conn->connected()) {
            conn->send(jsonStr);
        }
    }
}

void BackupWebSocketController::notifyJobUpdate(int job_id, const std::string& status, const Json::Value& stats) {
    Json::Value data;
    data["job_id"] = job_id;
    data["status"] = status;
    data["stats"] = stats;
    data["timestamp"] = std::time(nullptr);

    notifyAll("job_update", data);
}
