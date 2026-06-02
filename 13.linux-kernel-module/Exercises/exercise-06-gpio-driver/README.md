# Bài tập 06: GPIO Driver (Descriptor-based) ⭐⭐⭐

## 🎯 Mục tiêu

Viết module GPIO driver dùng descriptor-based API (`gpiod_*`): request GPIO, set direction, toggle value, và xử lý interrupt.

## 📋 Yêu cầu

1. Dùng `gpiod_get` hoặc `devm_gpiod_get` (KHÔNG `gpio_request` legacy API)
2. Set direction output với `gpiod_direction_output`
3. Toggle GPIO value với `gpiod_set_value` trong timer callback
4. Gắn interrupt với `gpiod_to_irq` + `request_irq` (hoặc `devm_request_irq`)
5. IRQ handler đếm số lần ngắt
6. Attribute sysfs để đọc trạng thái và số lần ngắt

## ✅ Tiêu chí đạt

- Module load thành công (cần GPIO simulated hoặc hardware)
- `cat /sys/kernel/gpio_driver/value` hiển thị trạng thái GPIO
- `cat /sys/kernel/gpio_driver/irq_count` hiển thị số lần ngắt
- Timer toggle GPIO mỗi 2 giây
- Trên host PC không có GPIO hardware, module có thể kiểm tra với GPIO mock

## 💡 Gợi ý

- Trên host PC không có GPIO thật, có thể dùng `gpio-mockup` kernel module để test
  ```bash
  sudo modprobe gpio-mockup gpio_mockup_ranges=0,8
  ```
- Trên target board, dùng `devm_gpiod_get(dev, "led", GPIOD_OUT_LOW)` để lấy GPIO từ DT
- `gpiod_set_value` nhận 0/1, tự động xử lý active-low
- `IRQF_TRIGGER_RISING` cho interrupt cạnh lên
- Timer callback toggle: `gpiod_set_value(gpio, !gpiod_get_value(gpio))`
- Có thể test trên host với `libgpiod` tools: `gpioset`, `gpioget`, `gpiomon`

## 🔍 Kiểm tra

```bash
# Trên host PC (không có GPIO hardware)
# Dùng GPIO mockup để test
sudo modprobe gpio-mockup gpio_mockup_ranges=0,8

# Test module (nếu GPIO number được hard-code cho mock)
sudo insmod gpio_driver.ko
dmesg | tail
cat /sys/kernel/gpio_driver/value
cat /sys/kernel/gpio_driver/irq_count
sudo rmmod gpio_driver
```

## 📁 File cần tạo

- [ ] `gpio_driver.c` - Source code GPIO driver
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-kernel-module.md`:
- Section 6.1 - 6.3: GPIO Descriptor-based API
- Section 7.2: Timers

---

**Bonus:** Thêm sysfs attribute `/sys/kernel/gpio_driver/toggle_ms` để điều chỉnh tốc độ toggle (ms).
