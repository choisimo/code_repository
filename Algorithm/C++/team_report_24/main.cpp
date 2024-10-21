//
// Created by csimo on 24. 10. 21.
//
#include "trafficFlow.h"
#include "csvLoader.h"


int main() {
    string filename = "C:/workspace/traffic_data_set.csv"; // CSV ���� ��� ����
    csvLoader loader(filename);
    vector<TrafficFlow> flows = loader.load();

    int count = 0;

    // TrafficFlow ������ ���
    for (const auto& flow : flows) {
       if (count == 2) break;
        cout << "Flow ID: " << flow.flowID
             << ", Source IP: " << flow.sourceIP
             << ", Source Port: " << flow.sourcePort
             << ", Destination IP: " << flow.destinationIP
             << ", Destination Port: " << flow.destinationPort
             << ", Protocol: " << flow.protocol
             << ", Timestamp: " << flow.timestamp
             << ", Flow Duration: " << flow.flowDuration
             << ", Total Forward Packets: " << flow.totalFwdPackets
             << ", Total Backward Packets: " << flow.totalBackwardPackets
             << ", Flow Bytes per Second: " << flow.flowBytesPerSec
             << ", Flow Packets per Second: " << flow.flowPacketsPerSec
             << ", Flow IAT Mean: " << flow.flowIATMean
             << ", Label: " << flow.label
             << ", Total Packets: " << flow.totalPackets
             << ", Protocol Name: " << flow.protocolName
             << "\n\n" << endl;
        count++;
    }

    cout << "Total Flow Count: " << count << endl;

    return 0;
}
