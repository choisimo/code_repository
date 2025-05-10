//
// Created by nodove on 2025-04-12.
//
#include "BackupManager.h"
#include <iostream>

BackupManager::BackupManager(const std::string& db_file) : db_path_(db_file) {
    // SQLite 데이터베이스 초기화
    sqlite3_open(db_path_.c_str(), &db_);
    sqlite3_exec(db_, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
}

BackupManager::~BackupManager() {
    if (db_) {
        sqlite3_close(db_);
    }
}

bool BackupManager::getBackupConfig(int config_id, std::map<std::string, std::string>& config) {
    std::lock_guard<std::mutex> lock(mutex_);

    const char* sql = "SELECT * FROM backup_config WHERE id = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, config_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int column_count = sqlite3_column_count(stmt);
        for (int i = 0; i < column_count; ++i) {
            const char* column_name = sqlite3_column_name(stmt, i);
            const char* column_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            config[column_name] = column_value ? column_value : "";
        }
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

bool BackupManager::stopBackupJob(int job_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = activeBackups_.find(job_id);
    if (it != activeBackups_.end()) {
        it->second->abort();
        activeBackups_.erase(it);

        // WebSocket 알림
        Json::Value data;
        data["message"] = "Backup job was manually stopped";
        BackupWebSocketController::notifyJobUpdate(job_id, "stopped", data);

        return true;
    }

    return false;
    }

    int startBackupJob(int config_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    try {
        // 설정 로드
        auto dbClient = drogon::app().getDbClient();
        auto result = dbClient->execSqlSync("SELECT * FROM backup_config WHERE id = ? AND is_active = 1", config_id);

        if (result.empty()) {
            throw std::runtime_error("Backup configuration not found or inactive");
        }

        // 백업 작업 레코드 생성
        auto jobResult = dbClient->execSqlSync(
            "INSERT INTO backup_jobs (config_id, status, start_time) VALUES (?, 'running', datetime('now')) RETURNING id",
            config_id
        );

        int job_id = jobResult[0]["id"].as<int>();

        // 비동기 백업 프로세스 시작
        auto process = std::make_shared<BackupProcess>(config_id, job_id);
        activeBackups_[job_id] = process;

        // 백업 실행 (별도 스레드에서)
        drogon::app().getThreadPool()->runTaskInQueue([this, process, job_id]() {
            try {
                // 진행 상황 보고 콜백 설정
                process->setProgressCallback([job_id](const Json::Value& stats) {
                    BackupWebSocketController::notifyJobUpdate(job_id, "running", stats);
                });

                // 백업 실행
                bool success = process->executeBackup();

                // 완료 처리
                process->completeBackup(success);

                // 완료 알림
                Json::Value finalStats = process->getStats();
                BackupWebSocketController::notifyJobUpdate(
                    job_id,
                    success ? "completed" : "failed",
                    finalStats
                );

                // 활성 작업 목록에서 제거
                std::lock_guard<std::mutex> lock(mutex_);
                activeBackups_.erase(job_id);

            } catch (const std::exception& e) {
                // 오류 처리
                process->logError(e.what());
                process->completeBackup(false);

                Json::Value errorStats;
                errorStats["error"] = e.what();
                BackupWebSocketController::notifyJobUpdate(job_id, "failed", errorStats);

                std::lock_guard<std::mutex> lock(mutex_);
                activeBackups_.erase(job_id);
            }
        });

        return job_id;

    } catch (const std::exception& e) {
        // 초기화 단계에서 오류 발생
        return -1;
    }}