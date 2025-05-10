//
// Created by nodove on 2025-04-12.
//
class BackupJobController : public drogon::HttpController<BackupJobController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(BackupJobController::getJobs, "/api/backup/jobs/{config_id}", drogon::Get);
    ADD_METHOD_TO(BackupJobController::startJob, "/api/backup/job/start/{config_id}", drogon::Post);
    ADD_METHOD_TO(BackupJobController::stopJob, "/api/backup/job/stop/{job_id}", drogon::Post);
    METHOD_LIST_END

    void getJobs(const drogon::HttpRequestPtr& req,
                 std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                 int config_id);

    void startJob(const drogon::HttpRequestPtr& req,
                  std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                  int config_id);

    void stopJob(const drogon::HttpRequestPtr& req,
                 std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                 int job_id);
};

// 백업 작업 시작 구현
void BackupJobController::startJob(const drogon::HttpRequestPtr& req,
                                  std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                                  int config_id) {
    // 비동기 작업으로 백업 시작
    drogon::app().getThreadPool()->runTaskInQueue([config_id, callback, this]() {
        try {
            // 백업 관리자를 통해 작업 시작
            int job_id = BackupManager::getInstance().startBackupJob(config_id);

            if (job_id > 0) {
                // WebSocket 이벤트 발송
                BackupWebSocketController::notifyAll(
                    "backup_started",
                    {{"job_id", job_id}, {"config_id", config_id}}
                );

                Json::Value ret;
                ret["status"] = "success";
                ret["job_id"] = job_id;
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                callback(resp);
            } else {
                Json::Value ret;
                ret["status"] = "error";
                ret["message"] = "Failed to start backup job";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                callback(resp);
            }
        } catch (const std::exception& e) {
            Json::Value ret;
            ret["status"] = "error";
            ret["message"] = e.what();
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            callback(resp);
        }
    });
}
