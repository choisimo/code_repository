// backup_monitor.js
class BackupMonitor {
    constructor() {
        this.socket = null;
        this.activeJobs = new Map();
        this.eventHandlers = {
            'init': this.handleInit.bind(this),
            'job_update': this.handleJobUpdate.bind(this),
            'error': this.handleError.bind(this)
        };

        this.connectWebSocket();
    }

    connectWebSocket() {
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const wsUrl = `${protocol}//${window.location.host}/ws/backup_status`;

        this.socket = new WebSocket(wsUrl);

        this.socket.onopen = () => {
            console.log('WebSocket connection established');
            this.updateConnectionStatus('connected');
        };

        this.socket.onmessage = (event) => {
            try {
                const message = JSON.parse(event.data);
                const handler = this.eventHandlers[message.event];

                if (handler) {
                    handler(message.data);
                } else {
                    console.warn(`Unknown event type: ${message.event}`);
                }
            } catch (e) {
                console.error('Error parsing WebSocket message:', e);
            }
        };

        this.socket.onclose = () => {
            console.log('WebSocket connection closed');
            this.updateConnectionStatus('disconnected');

            // 재연결 시도
            setTimeout(() => this.connectWebSocket(), 5000);
        };

        this.socket.onerror = (error) => {
            console.error('WebSocket error:', error);
            this.updateConnectionStatus('error');
        };
    }

    handleInit(data) {
        // 초기 활성 작업 설정
        const activeJobs = data.active_jobs || [];

        this.activeJobs.clear();
        for (const job of activeJobs) {
            this.activeJobs.set(job.id, job);
            this.updateJobUI(job.id, job);
        }
    }

    handleJobUpdate(data) {
        const jobId = data.job_id;
        const status = data.status;
        const stats = data.stats || {};

        // 작업 상태 업데이트
        if (status === 'running') {
            this.activeJobs.set(jobId, {
                id: jobId,
                status: status,
                stats: stats
            });
        } else {
            // 작업 완료 또는 실패
            this.activeJobs.delete(jobId);
        }

        // UI 업데이트
        this.updateJobUI(jobId, {
            status: status,
            stats: stats
        });

        // 알림 표시
        if (status === 'completed' || status === 'failed') {
            this.showNotification(
                `Backup job #${jobId} ${status}`,
                `Total files: ${stats.total_files}, Success: ${stats.success_count}, Failed: ${stats.fail_count}`
            );
        }
    }

    handleError(data) {
        console.error('WebSocket error:', data.message);
        this.showNotification('Error', data.message, 'error');
    }

    updateConnectionStatus(status) {
        const statusElem = document.getElementById('connection-status');
        if (statusElem) {
            statusElem.className = `status-indicator ${status}`;
            statusElem.textContent = status.charAt(0).toUpperCase() + status.slice(1);
        }
    }

    updateJobUI(jobId, jobData) {
        // 작업 UI 요소 찾기 또는 생성
        let jobElem = document.getElementById(`job-${jobId}`);

        if (!jobElem && (jobData.status === 'running' || jobData.status === 'pending')) {
            // 새 작업 UI 생성
            jobElem = this.createJobElement(jobId);
            document.getElementById('active-jobs-container').appendChild(jobElem);
        }

        if (jobElem) {
            // 상태 업데이트
            const statusElem = jobElem.querySelector('.job-status');
            if (statusElem) {
                statusElem.textContent = jobData.status;
                statusElem.className = `job-status ${jobData.status}`;
            }

            // 통계 업데이트
            if (jobData.stats) {
                const stats = jobData.stats;

                // 진행률 업데이트
                const progressBar = jobElem.querySelector('.progress-bar');
                if (progressBar && stats.progress_percent !== undefined) {
                    progressBar.style.width = `${stats.progress_percent}%`;
                    progressBar.textContent = `${stats.progress_percent}%`;
                }

                // 기타 통계 업데이트
                const statsElem = jobElem.querySelector('.job-stats');
                if (statsElem) {
                    statsElem.innerHTML = `
                        <div>Files: ${stats.total_files || 0}</div>
                        <div>Size: ${this.formatBytes(stats.total_size || 0)}</div>
                        <div>Success: ${stats.success_count || 0}</div>
                        <div>Failed: ${stats.fail_count || 0}</div>
                        <div>Duration: ${this.formatDuration(stats.duration_seconds || 0)}</div>
                    `;
                }
            }

            // 완료된 작업 처리
            if (jobData.status === 'completed' || jobData.status === 'failed' || jobData.status === 'stopped') {
                // 완료된 작업을 히스토리 영역으로 이동
                setTimeout(() => {
                    if (jobElem.parentNode) {
                        jobElem.parentNode.removeChild(jobElem);
                    }
                    this.addJobToHistory(jobId, jobData);
                }, 3000);
            }
        }
    }

    createJobElement(jobId) {
        const jobElem = document.createElement('div');
        jobElem.id = `job-${jobId}`;
        jobElem.className = 'job-card';

        jobElem.innerHTML = `
            <div class="job-header">
                <h3>Backup Job #${jobId}</h3>
                <span class="job-status pending">Pending</span>
            </div>
            <div class="progress-container">
                <div class="progress-bar" style="width:0%">0%</div>
            </div>
            <div class="job-stats">
                <div>Files: 0</div>
                <div>Size: 0 B</div>
                <div>Success: 0</div>
                <div>Failed: 0</div>
                <div>Duration: 0s</div>
            </div>
            <div class="job-actions">
                <button class="btn-stop" onclick="backupMonitor.stopJob(${jobId})">Stop</button>
                <button class="btn-view-logs" onclick="backupMonitor.viewLogs(${jobId})">Logs</button>
            </div>
        `;

        return jobElem;
    }

    addJobToHistory(jobId, jobData) {
        // 히스토리 목록에 작업 추가
        // 구현 생략
    }

    stopJob(jobId) {
        fetch(`/api/backup/job/stop/${jobId}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            }
        })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    this.showNotification('Success', `Job #${jobId} has been stopped`);
                } else {
                    this.showNotification('Error', data.message, 'error');
                }
            })
            .catch(error => {
                this.showNotification('Error', error.message, 'error');
            });
    }

    viewLogs(jobId) {
        window.open(`/backup/logs/${jobId}`, '_blank');
    }

    startBackup(configId) {
        fetch(`/api/backup/job/start/${configId}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            }
        })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    this.showNotification('Success', `Backup job started with ID: ${data.job_id}`);
                } else {
                    this.showNotification('Error', data.message, 'error');
                }
            })
            .catch(error => {
                this.showNotification('Error', error.message, 'error');
            });
    }

    formatBytes(bytes, decimals = 2) {
        if (bytes === 0) return '0 B';

        const k = 1024;
        const dm = decimals < 0 ? 0 : decimals;
        const sizes = ['B', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];

        const i = Math.floor(Math.log(bytes) / Math.log(k));

        return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
    }

    formatDuration(seconds) {
        const hours = Math.floor(seconds / 3600);
        const minutes = Math.floor((seconds % 3600) / 60);
        const secs = seconds % 60;

        return [
            hours > 0 ? hours + 'h' : '',
            minutes > 0 ? minutes + 'm' : '',
            secs + 's'
        ].filter(Boolean).join(' ');
    }

    showNotification(title, message, type = 'info') {
        // 알림 표시 구현
        // 구현 생략
    }
}

// 글로벌 인스턴스 생성
const backupMonitor = new BackupMonitor();
