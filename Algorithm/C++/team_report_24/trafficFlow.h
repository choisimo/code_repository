//
// Created by csimo on 24. 10. 21.
//

#ifndef TRAFFICFLOW_H
#define TRAFFICFLOW_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

struct TrafficFlow {
    std::string flowID;            // 트래픽 흐름 ID
    std::string sourceIP;          // 송신 IP
    int sourcePort;                // 송신 포트 번호
    std::string destinationIP;     // 수신 IP
    int destinationPort;           // 수신 포트 번호
    int protocol;                  // 프로토콜 (예: TCP = 6, UDP = 17)
    std::string timestamp;         // 타임스탬프
    double flowDuration;           // 트래픽 흐름의 지속 시간
    int totalFwdPackets;           // 총 송신된 패킷 수
    int totalBackwardPackets;      // 총 수신된 패킷 수
    double flowBytesPerSec;        // 초당 바이트 전송량
    double flowPacketsPerSec;      // 초당 패킷 전송량
    double flowIATMean;            // 패킷 간 평균 간격
    std::string label;             // 트래픽 라벨 (정상, 비정상)
    int totalPackets;              // 총 패킷 수
    std::string protocolName;      // 프로토콜 이름
};

#endif //TRAFFICFLOW_H
