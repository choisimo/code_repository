//
// Created by nodove on 2025-04-12.
//
class BackupWebInterface {
private:
    BackupManager db_manager;

public:
    BackupWebInterface(const std::string& db_path) : db_manager(db_path) {}

    // 웹에서 호출할 API 함수들

    // 백업 설정 목록 가져오기
    std::string getBackupConfigurations() {
        // JSON 형태로 결과 반환
        std::string result = "{ \"configs\": [";

        const char* sql = "SELECT id, name, schedule, backup_time, source_dir, dest_dir, backup_type, "
                         "remote_server, is_active FROM backup_config ORDER BY id";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db_manager.getDb(), sql, -1, &stmt, nullptr);

        bool first = true;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) {
                result += ",";
            }
            first = false;

            result += "{";
            result += "\"id\":" + std::to_string(sqlite3_column_int(stmt, 0)) + ",";
            result += "\"name\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) + "\",";
            result += "\"schedule\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) + "\",";
            result += "\"backup_time\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))) + "\",";
            result += "\"source_dir\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))) + "\",";
            result += "\"dest_dir\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))) + "\",";
            result += "\"backup_type\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))) + "\",";

            const char* remote = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
            result += "\"remote_server\":\"" + (remote ? escapeJson(remote) : "") + "\",";

            result += "\"is_active\":" + std::to_string(sqlite3_column_int(stmt, 8));
            result += "}";
        }

        sqlite3_finalize(stmt);
        result += "]}";

        return result;
    }

    // 백업 작업 실행
    std::string runBackup(int config_id) {
        try {
            // 백업 프로세스 객체 생성
            BackupProcess process(db_manager, config_id);

            // 백업 실행 (동기 방식)
            bool success = process.executeBackup();

            // 결과 반환
            return "{ \"success\": " + std::string(success ? "true" : "false") +
                   ", \"job_id\": " + std::to_string(process.getJobId()) + " }";
        }
        catch (const std::exception& e) {
            return "{ \"success\": false, \"error\": \"" + escapeJson(e.what()) + "\" }";
        }
    }

    // 백업 작업 이력 가져오기
    std::string getBackupJobHistory(int config_id, int limit = 10) {
        std::string result = "{ \"jobs\": [";

        const char* sql = "SELECT id, status, start_time, end_time, duration_seconds, "
                         "total_files, total_size, success_count, fail_count, log_file "
                         "FROM backup_jobs WHERE config_id = ? "
                         "ORDER BY start_time DESC LIMIT ?";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db_manager.getDb(), sql, -1, &stmt, nullptr);

        sqlite3_bind_int(stmt, 1, config_id);
        sqlite3_bind_int(stmt, 2, limit);

        bool first = true;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) {
                result += ",";
            }
            first = false;

            result += "{";
            result += "\"id\":" + std::to_string(sqlite3_column_int(stmt, 0)) + ",";
            result += "\"status\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))) + "\",";
            result += "\"start_time\":\"" + escapeJson(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) + "\",";

            const char* end_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            result += "\"end_time\":\"" + (end_time ? escapeJson(end_time) : "") + "\",";

            result += "\"duration_seconds\":" + std::to_string(sqlite3_column_int(stmt, 4)) + ",";
            result += "\"total_files\":" + std::to_string(sqlite3_column_int(stmt, 5)) + ",";
            result += "\"total_size\":" + std::to_string(sqlite3_column_int64(stmt, 6)) + ",";
            result += "\"success_count\":" + std::to_string(sqlite3_column_int(stmt, 7)) + ",";
            result += "\"fail_count\":" + std::to_string(sqlite3_column_int(stmt, 8)) + ",";

            const char* log_file = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
            result += "\"log_file\":\"" + (log_file ? escapeJson(log_file) : "") + "\"";

            result += "}";
        }

        sqlite3_finalize(stmt);
        result += "]}";

        return result;
    }

private:
    std::string escapeJson(const std::string& input) {
        std::string result;
        for (char c : input) {
            switch (c) {
                case '\"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\b': result += "\\b"; break;
                case '\f': result += "\\f"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default:
                    if (c < 32) {
                        char buf[8];
                        snprintf(buf, sizeof(buf), "\\u%04x", c);
                        result += buf;
                    } else {
                        result += c;
                    }
            }
        }
        return result;
    }
};
