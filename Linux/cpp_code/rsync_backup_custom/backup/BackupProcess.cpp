//
// Created by nodove on 2025-04-12.
//

class BackupProcess {
private:
    int config_id_;
    int job_id_;
    std::map<std::string, std::string> config_;

    // 통계 관련 멤버
    std::atomic<int> total_files_{0};
    std::atomic<long> total_size_{0};
    std::atomic<int> success_count_{0};
    std::atomic<int> fail_count_{0};
    std::atomic<int> current_progress_{0};
    std::atomic<bool> aborted_{false};

    std::chrono::time_point<std::chrono::system_clock> start_time_;
    std::string log_file_;

    // 진행 상황 콜백
    std::function<void(const Json::Value&)> progress_callback_;

    // 프로그레스 업데이트 타이머
    trantor::TimerId progress_timer_id_;

public:
    BackupProcess(int config_id, int job_id)
        : config_id_(config_id), job_id_(job_id) {

        loadConfig();
        setupLogFile();

        // 시작 시간 기록
        start_time_ = std::chrono::system_clock::now();

        // 프로그레스 보고 타이머 설정 (1초마다)
        auto loop = trantor::EventLoop::getEventLoopOfCurrentThread();
        progress_timer_id_ = loop->runEvery(1.0, [this]() {
            reportProgress();
        });
    }

    ~BackupProcess() {
        // 타이머 중지
        auto loop = trantor::EventLoop::getEventLoopOfCurrentThread();
        loop->invalidateTimer(progress_timer_id_);
    }

    void setProgressCallback(std::function<void(const Json::Value&)> callback) {
        progress_callback_ = std::move(callback);
    }

    bool executeBackup() {
        try {
            // 로그 시작
            logInfo("Starting backup job for config: " + std::to_string(config_id_));

            // 백업 명령 구성
            std::string source_dir = config_["source_dir"];
            std::string dest_dir = config_["dest_dir"];
            std::string backup_type = config_["backup_type"];
            std::string rsync_cmd = buildRsyncCommand();

            // 명령 실행 로깅
            logInfo("Executing command: " + rsync_cmd);

            // rsync 명령 실행
            FILE* pipe = popen(rsync_cmd.c_str(), "r");
            if (!pipe) {
                logError("Failed to execute rsync command");
                return false;
            }

            // 결과 처리
            processRsyncOutput(pipe);
            int exit_code = pclose(pipe);

            // 중간에 취소됐는지 확인
            if (aborted_) {
                logWarning("Backup job was aborted");
                return false;
            }

            // 종료 코드 확인
            if (exit_code != 0) {
                logError("Rsync exited with code: " + std::to_string(exit_code));
                return false;
            }

            logInfo("Backup completed successfully");
            return true;
        }
        catch (const std::exception& e) {
            logError(std::string("Exception during backup: ") + e.what());
            return false;
        }
    }

    void abort() {
        aborted_ = true;
        // TODO: 백업 프로세스 강제 종료 로직
    }

    void completeBackup(bool success) {
        auto end_time = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(
            end_time - start_time_).count();

        // DB 업데이트
        auto dbClient = drogon::app().getDbClient();
        dbClient->execSqlAsync(
            "UPDATE backup_jobs SET "
            "status = ?, end_time = datetime('now'), duration_seconds = ?, "
            "total_files = ?, total_size = ?, success_count = ?, fail_count = ?, "
            "log_file = ? WHERE id = ?",
            success ? "completed" : "failed",
            duration,
            total_files_.load(),
            total_size_.load(),
            success_count_.load(),
            fail_count_.load(),
            log_file_,
            job_id_
        );

        // 이메일 알림 발송 (비동기로)
        sendEmailNotification(success);
    }

    Json::Value getStats() {
        Json::Value stats;
        stats["total_files"] = total_files_.load();
        stats["total_size"] = total_size_.load();
        stats["success_count"] = success_count_.load();
        stats["fail_count"] = fail_count_.load();

        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(
            now - start_time_).count();
        stats["duration_seconds"] = static_cast<int>(duration);

        stats["progress_percent"] = current_progress_.load();

        return stats;
    }

    void logInfo(const std::string& message) {
        logMessage("info", message);
    }

    void logWarning(const std::string& message) {
        logMessage("warning", message);
    }

    void logError(const std::string& message) {
        logMessage("error", message);
    }

private:
    void loadConfig() {
        auto dbClient = drogon::app().getDbClient();
        auto result = dbClient->execSqlSync("SELECT * FROM backup_config WHERE id = ?", config_id_);

        if (result.empty()) {
            throw std::runtime_error("Backup configuration not found");
        }

        const auto& row = result[0];
        for (auto columnName : row.columnNames()) {
            config_[columnName] = row[columnName].as<std::string>();
        }
    }

    void setupLogFile() {
        std::string log_dir = config_["log_dir"];
        std::string timestamp = getCurrentTimestamp();
        log_file_ = log_dir + "/backup_" + timestamp + "_job" + std::to_string(job_id_) + ".log";

        // 로그 디렉토리 확인
        std::filesystem::create_directories(log_dir);
    }

    std::string buildRsyncCommand() {
        std::string cmd = "rsync " + config_["sync_options"] + " ";

        // 백업 유형에 따른 설정
        if (config_["backup_type"] == "incremental") {
            // 증분 백업 설정
            std::string last_backup_dir = getLastBackupDir();
            if (!last_backup_dir.empty()) {
                cmd += "--link-dest=" + last_backup_dir + " ";
            }
        }

        // 원격 서버 사용 시 설정
        if (!config_["remote_server"].empty()) {
            std::string ssh_options = buildSshOptions();
            cmd += "-e \"ssh " + ssh_options + "\" ";

            cmd += "\"" + config_["source_dir"] + "\" " +
                   config_["remote_server"] + ":" +
                   "\"" + config_["dest_dir"] + "\"";
        } else {
            cmd += "\"" + config_["source_dir"] + "\" \"" + config_["dest_dir"] + "\"";
        }

        return cmd;
    }

    std::string buildSshOptions() {
        std::string options;

        if (!config_["remote_port"].empty()) {
            options += "-p " + config_["remote_port"] + " ";
        }

        if (!config_["identity_file"].empty()) {
            options += "-i " + config_["identity_file"] + " ";
        }

        return options;
    }

    void processRsyncOutput(FILE* pipe) {
        char buffer[4096];

        while (!aborted_ && fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::string line(buffer);

            // 로그 파일에 추가
            appendToLogFile(line);

            // 진행 상황 파싱 및 업데이트
            parseRsyncProgress(line);
        }
    }

    void parseRsyncProgress(const std::string& line) {
        // rsync 출력에서 파일 정보 및 진행 상황 추출
        // 기본적인 구현 - 실제로는 더 복잡한 파싱 필요

        // 예: 파일 전송 정보 라인
        if (line.find("->") != std::string::npos) {
            total_files_++;
            success_count_++;

            // 파일 크기 추출 시도
            size_t size_pos = line.find_first_of(" ");
            if (size_pos != std::string::npos) {
                try {
                    std::string size_str = line.substr(0, size_pos);
                    long file_size = std::stol(size_str);
                    total_size_ += file_size;
                } catch (...) {
                    // 파싱 오류 무시
                }
            }
        }

        // 진행률 표시 (예: "32% 151.35MB/s 0:01:28")
        if (line.find("%") != std::string::npos) {
            try {
                size_t percent_pos = line.find("%");
                if (percent_pos != std::string::npos && percent_pos > 0) {
                    std::string percent_str = line.substr(percent_pos - 3, 3);
                    current_progress_ = std::stoi(percent_str);
                }
            } catch (...) {
                // 파싱 오류 무시
            }
        }
    }

    void reportProgress() {
        if (progress_callback_) {
            progress_callback_(getStats());
        }
    }

    void logMessage(const std::string& level, const std::string& message) {
        // DB에 로그 기록
        auto dbClient = drogon::app().getDbClient();
        dbClient->execSqlAsync(
            "INSERT INTO backup_logs (job_id, timestamp, log_level, message) "
            "VALUES (?, datetime('now'), ?, ?)",
            job_id_, level, message
        );

        // 로그 파일에도 기록
        std::string timestamp = getCurrentTimestamp("%Y-%m-%d %H:%M:%S");
        std::string log_line = timestamp + " [" + level + "] " + message + "\n";
        appendToLogFile(log_line);
    }

    void appendToLogFile(const std::string& text) {
        std::ofstream log_file(log_file_, std::ios_base::app);
        if (log_file.is_open()) {
            log_file << text;
            log_file.close();
        }
    }

    std::string getLastBackupDir() {
        // 마지막 성공 백업 디렉토리 찾기
        // 구현 생략
        return "";
    }

    void sendEmailNotification(bool success) {
        // 이메일 알림 전송 로직
        // 구현 생략
    }

    std::string getCurrentTimestamp(const std::string& format = "%Y%m%d_%H%M%S") {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);

        std::tm tm_now;
        localtime_r(&time_t_now, &tm_now);

        char buffer[64];
        std::strftime(buffer, sizeof(buffer), format.c_str(), &tm_now);

        return std::string(buffer);
    }
};
