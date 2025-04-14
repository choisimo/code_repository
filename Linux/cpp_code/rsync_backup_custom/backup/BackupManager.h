//
// Created by nodove on 4/11/25.
//
#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

class BackupManager {
private:
    sqlite3* db_;
    std::string db_path_;
    std::unordered_map<int, std::shared_ptr<BackupProcess>> activeBackups_;
    std::mutex mutex_;

    BackupManager() = default;

public:
    BackupManager(const std::string& db_file);
    ~BackupManager();
    bool getBackupConfig(int config_id, std::map<std::string, std::string>& config);
    std::vector<std::string> getNotificationEmails(int config_id);
    int startBackupJob(int config_id);
    bool stopBackupJob(int job_id);
};