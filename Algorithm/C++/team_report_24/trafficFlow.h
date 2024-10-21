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
    std::string flowID;            // Ʈ���� �帧 ID
    std::string sourceIP;          // �۽� IP
    int sourcePort;                // �۽� ��Ʈ ��ȣ
    std::string destinationIP;     // ���� IP
    int destinationPort;           // ���� ��Ʈ ��ȣ
    int protocol;                  // �������� (��: TCP = 6, UDP = 17)
    std::string timestamp;         // Ÿ�ӽ�����
    double flowDuration;           // Ʈ���� �帧�� ���� �ð�
    int totalFwdPackets;           // �� �۽ŵ� ��Ŷ ��
    int totalBackwardPackets;      // �� ���ŵ� ��Ŷ ��
    double flowBytesPerSec;        // �ʴ� ����Ʈ ���۷�
    double flowPacketsPerSec;      // �ʴ� ��Ŷ ���۷�
    double flowIATMean;            // ��Ŷ �� ��� ����
    std::string label;             // Ʈ���� �� (����, ������)
    int totalPackets;              // �� ��Ŷ ��
    std::string protocolName;      // �������� �̸�
};

#endif //TRAFFICFLOW_H
