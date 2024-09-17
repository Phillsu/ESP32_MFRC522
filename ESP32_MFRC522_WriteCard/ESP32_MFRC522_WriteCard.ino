#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 4

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();    // 初始化 SPI
  rfid.PCD_Init(); // 初始化 MFRC522
  Serial.println("將卡片靠近讀取器...");
}

void loop() {
  // 搜尋新卡
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // 選擇卡片
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // 卡片的 UID
  Serial.print("卡片 UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // 儲存的數據
  byte data[16] = {0};
  String newId = "12345678";
  for (int i = 0; i < newId.length(); i++) {
    data[i] = newId[i];
  }

  // 要寫入的區塊
  byte blockAddr = 4; // 設定要寫入的區塊（例如區塊 4）

  // 檢查是否可以寫入
  MFRC522::StatusCode status = rfid.MIFARE_Write(blockAddr, data, 16);
  if (status == MFRC522::STATUS_OK) {
    Serial.println("成功寫入數據！");
  } else {
    Serial.print("寫入失敗：");
    Serial.println(status);
  }

  rfid.PICC_HaltA(); // 停止與卡片的通訊
  delay(2000); // 等待 2 秒鐘
}
