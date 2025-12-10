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

