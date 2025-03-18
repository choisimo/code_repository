const readline = require('readline');

// readline 인터페이스 생성
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

console.log("Enter Steam Workshop URLs (type 'stop' to finish):");

const ids = []; // ID를 저장할 배열

// URL에서 ID를 추출하는 함수
function extractIdFromUrl(url) {
  const match = url.match(/id=(\d+)/);
  return match ? match[1] : null;
}

// 입력 처리
rl.on('line', (input) => {
  if (input.toLowerCase() === 'stop') {
    // stop 입력 시 종료
    console.log("Collected IDs:", ids);
    console.log("Collectd IDS:", ids.join(';'));
    rl.close();
  } else {
    const id = extractIdFromUrl(input);
    if (id) {
      ids.push(id);
      console.log(`Extracted ID: ${id}`);
    } else {
      console.log("Invalid URL. Please enter a valid Steam Workshop URL.");
    }
  }
});

rl.on('close', () => {
  console.log("Program terminated.");
  process.exit(0);
});
