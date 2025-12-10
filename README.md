# TimeGuard Access Shield ⏳🔐  
### Password-Controlled Time-Restricted Machine Operation System  

The **TimeGuard Access Shield** is a secure embedded system that enables or disables machine operation based on password authentication and predefined working hours using RTC. It prevents unauthorized usage and ensures safe scheduled access.

---

## 🔥 Features  

- Real-time Clock display
- Password authentication using matrix keypad
- Machine allowed only inside selected time slot
- Config menu using external interrupt
- Edit RTC settings 
- Edit allowed operation timing
- Edit password
- Secure and flexible control system  

---

## 🛠 Hardware Requirements  

- LPC2148 ARM7 Microcontroller
- 16x2 LCD
- 4x4 Matrix Keypad
- Switch (Interrupt input)
- Relay / LED for Device ON/OFF
- RTC
- USB-UART (for programming)

---

## 💻 Software Requirements  

- **Keil uVision**
- Embedded-C
- Flash Magic Programmer  

---

## 🧩 Working Principle  

1. System continuously displays RTC on LCD  
2. User presses entry button  
3. User enters password  
4. If password is valid → Check time range  
5. If inside schedule → Device turns ON  
6. Outside timing → Access denied  
7. External switch allows:
   - RTC edit
   - password edit
   - working hours modification  

---

## 📸 Project Hardware Preview  

> My real hardware implementation on Vector’s Advanced Development Board (ARM7)

### 🔹 LPC2148 Base Board
![IMG20251210171924](https://github.com/user-attachments/assets/4a2dde39-e2fd-489b-b1b1-fa61fcc960b3)


### 🔹 LCD + RTC Working
![IMG20251210173329](https://github.com/user-attachments/assets/2ca62a6c-1433-4bc1-b34a-709212411064)


### 🔹 Time Not Allowed Screen
![IMG20251210173454](https://github.com/user-attachments/assets/2bc22f03-c189-4539-a02b-b3543f1b31ad)


### 🔹 Main Menu on LCD
![IMG20251210174517](https://github.com/user-attachments/assets/9ff5bf71-ba83-48f4-9d5a-47b6444ae990)


### 🔹 RTC Edit Menu
![IMG20251210174610](https://github.com/user-attachments/assets/dca9cb1d-f6e7-4249-a4d0-ab95f5d7c891)

