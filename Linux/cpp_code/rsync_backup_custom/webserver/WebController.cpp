//
// Created by nodove on 2025-04-12.
//
#pragma once
#include <drogon/HttpController.h>
#include "BackupManager.h"

class BackupConfigController : public drogon::HttpController<BackupConfigController> {
public:
    METHOD_LIST_BEGIN
    // 백업 설정 관련 API 엔드포인트
    ADD_METHOD_TO(BackupConfigController::getAll, "/api/backup/configs", drogon::Get);
    ADD_METHOD_TO(BackupConfigController::getOne, "/api/backup/config/{id}", drogon::Get);
    ADD_METHOD_TO(BackupConfigController::create, "/api/backup/config", drogon::Post);
    ADD_METHOD_TO(BackupConfigController::update, "/api/backup/config/{id}", drogon::Put);
    ADD_METHOD_TO(BackupConfigController::remove, "/api/backup/config/{id}", drogon::Delete);
    METHOD_LIST_END

    void getAll(const drogon::HttpRequestPtr& req,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void getOne(const drogon::HttpRequestPtr& req,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                int id);

    void create(const drogon::HttpRequestPtr& req,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void update(const drogon::HttpRequestPtr& req,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                int id);

    void remove(const drogon::HttpRequestPtr& req,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                int id);
};

// API 구현 예시 (getAll 메서드)
void BackupConfigController::getAll(const drogon::HttpRequestPtr& req,
                                   std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync("SELECT * FROM backup_config WHERE is_active = 1",
        [callback](const drogon::orm::Result& result) {
            Json::Value ret;
            ret["status"] = "success";
            ret["configs"] = Json::Value(Json::arrayValue);

            for (const auto& row : result) {
                Json::Value config;
                config["id"] = row["id"].as<int>();
                config["name"] = row["name"].as<std::string>();
                config["schedule"] = row["schedule"].as<std::string>();
                config["backup_time"] = row["backup_time"].as<std::string>();
                config["source_dir"] = row["source_dir"].as<std::string>();
                config["dest_dir"] = row["dest_dir"].as<std::string>();
                config["backup_type"] = row["backup_type"].as<std::string>();
                // 나머지 필드들도 추가

                ret["configs"].append(config);
            }

            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            Json::Value ret;
            ret["status"] = "error";
            ret["message"] = e.base().what();
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            callback(resp);
        });
}
