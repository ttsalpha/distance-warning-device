#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Khai báo thư viện của chuẩn giao tiếp I2C với led LCD
#define buzz 10                // Khai báo chân điều khiển tín hiệu của loa là chân số 10 của arduino

LiquidCrystal_I2C lcd(0x27, 16, 2); // khai báo địa chỉ mặc định và kích thước hiển thị của LCD
const int trig = 3;                 // Khai báo dạng int chân 3 của arduino tên là trig,
const int echo = 2;                 // Khai báo dạng int chân 4 của arduino tên là echo.

void setup()
{
    Serial.begin(9600);
    lcd.init(); // Khởi động màn hình LCD, bắt đầu cho phép Arduino sử dụng màn hình
    lcd.begin(16, 2);
    lcd.backlight();       // Bật đèn nền LCD 16×2.
    pinMode(trig, OUTPUT); // cổng 3 của arduino để phát tín hiệu.
    pinMode(echo, INPUT);  // cổng 2 của arduino để nhận tín hiệu đo được của HC-SR04.
    pinMode(buzz, OUTPUT); // cổng 10 của arduino định nghĩa cổng ra để phát tín hiệu cho loa hoạt động
}

void loop()
{
    float duration;           // Khai báo biến thời gian để tính toán
    float distance;           // Khai báo biến khoảng cách
    digitalWrite(trig, LOW);  // đặt cho tín hiệu phát ra của HC-SR04 là LOW (0)
    delayMicroseconds(2);     // giữ nguyên trong 2 µs
    digitalWrite(trig, HIGH); // đặt cho tín hiệu phát ra của HC-SR04 là HIGH (1)
    delayMicroseconds(10);    // giữ nguyên trong 10 µs
    digitalWrite(trig, LOW);  // đặt cho tín hiệu phát ra của HC-SR04 là LOW (0)
    /* Phần trên là để tạo 1 xung đơn giản nhằm đo lường khoảng thời gian từ khi phát đến khi nhận được tín hiệu cao tại chân echo */
    duration = pulseIn(echo, HIGH, 30000); // hàm này là để đo thời gian xung tại chân echo có giá trị là HIGH, nếu quá 30000 ms thì sẽ trả về là 0
    distance = (duration * 0.034 / 2);     // tính toán để ra số khoảng cách bằng cách nhân với tốc độ sóng âm trong không khí và chia đôi quãng đường truyền.
    lcd.setCursor(0, 0);                   // đưa con trỏ của LCD về vị trí 0, 0
    lcd.print("Distance:");
    lcd.print(distance); // in ra khoảng cách
    lcd.setCursor(14, 0);
    lcd.print("cm");
    if (distance < 10 && distance > 0) // nếu khoảng cách nhỏ hơn 10 cm
    {
        digitalWrite(buzz, 1);         // đặt còi buzz phát âm thanh liên tục
        lcd.setCursor(0, 1);           // đưa con trỏ của LCD về vị trí 0, 1 (đầu tiên của dòng 2)
        lcd.print("    STOP !!!    "); // hiển thị thông báo STOP !!!
    }
    else if (distance > 10 && distance < 30) // nếu khoảng cách lớn hơn 10 và nhỏ hơn 30cm
    {
        digitalWrite(buzz, 1);         // đặt còi buzz phát âm thanh liên tục
        delay(500);                    // giữ còi phát âm thanh trong 0.5 s
        digitalWrite(buzz, 0);         // tắt âm thanh còi buzz sẽ kêu trong 0,5 s và tắt trong 0,5 s
        lcd.setCursor(0, 1);           // đưa con trỏ của LCD về vị trí 0, 1 (đầu tiên của dòng 2)
        lcd.print("    TOO NEAR    "); // hiển thị thông báo TOO NEAR
    }
    else // nếu vật thể ngoài khoảng thông báo
    {
        digitalWrite(buzz, 0);         // còi Buzz không kêu
        lcd.setCursor(0, 1);           // đưa con trỏ của LCD về vị trí 0, 1 (đầu tiên của dòng 2)
        lcd.print("     CLEAR      "); // hiển thị thông báo CLEAR
    }
    delay(500); // chờ trong 0,5s rồi lại tiếp tục lặp lại vòng loop.
}
