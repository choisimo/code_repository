import os

# 생성할 디렉토리
output_dir = os.path.join(os.path.dirname(__file__), 'test-logs')
os.makedirs(output_dir, exist_ok=True)

# 패턴별 샘플 로그
samples = {
    'apache.log': (
        '127.0.0.1 - - [10/Oct/2020:13:55:36 -0700] "GET /index.html HTTP/1.1" 200 2326\n'
        '127.0.0.1 - - [10/Oct/2020:13:56:01 -0700] "POST /submit HTTP/1.1" 404 721\n'
    ),
    'nginx.log': (
        '192.168.0.1 - user1 [10/Oct/2020:13:57:12 +0000] "GET /status HTTP/1.1" 200 512 "-" "curl/7.68.0"\n'
    ),
    'syslog_bsd.log': (
        '<34>Oct 11 22:14:15 myhost myapp[123]: Example BSD syslog message\n'
    ),
    'syslog_rfc5424.log': (
        '<34>1 2020-10-11T22:14:15Z myhost myapp 1234 ID47 [exampleSDID@32473 iut="3" eventSource="App"] Test RFC5424 message\n'
    ),
    'windows_event.log': (
        '2020-10-11T22:14:15.123Z 4624 INFORMATION Security Successful login for user\n'
    ),
    'cef.log': (
        'CEF:0|Security|threatmanager|1.0|100|Port Scan|10| src=10.0.0.1 dst=10.0.0.2 spt=1232\n'
    ),
    'gelf.log': (
        '{"version":"1.1", "host":"example.org", "short_message":"GELF log message"}\n'
    ),
    'aws_cloudtrail.log': (
        '2020-10-11T22:14:15Z ec2.amazonaws.com RunInstances\n'
    ),
    'iis_w3c.log': (
        '2020-10-11 22:14:15 192.168.0.1 GET /home.html - 80\n'
    ),
    'docker_json.log': (
        '{"log":"Container started","stream":"stdout","time":"2020-10-11T22:14:15Z"}\n'
    ),
    'custom_app.log': (
        '[2020-10-11 22:14:15,123] INFO MainThread my.logger - Application started\n'
    ),
    'cisco_asa.log': (
        '%ASA-6-600: Built outbound TCP connection\n'
    ),
    'k8s_pod.log': (
        '2020-10-11T22:14:15.123Z [INFO] Pod started successfully\n'
    ),
    'elf.log': (
        '#Version: 1.0\n#Fields: date time cs-method cs-uri-stem cs-uri-query cs(Referer)\n'
        '2020-10-11 22:14:15 GET /index.html - example.com\n'
    ),
    'w3c_extended.log': (
        '2020-10-11 22:14:15 192.168.0.1 GET /contact.html - 443\n'
    ),
    'heroku.log': (
        '2020-10-11T22:14:15.123Z heroku web.1 - Starting process with ID 1234\n'
    ),
    'aws_cloudwatch.log': (
        '[2020-10-11T22:14:15.123Z] INFO Application started\n'
    ),
}

for fname, content in samples.items():
    path = os.path.join(output_dir, fname)
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)

print(f"Test logs created at: {output_dir}")
