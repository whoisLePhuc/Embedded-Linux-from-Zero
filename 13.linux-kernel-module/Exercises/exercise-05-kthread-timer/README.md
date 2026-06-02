# Bài tập 05: Kernel Thread và Timer ⭐⭐⭐

## 🎯 Mục tiêu

Viết module spawn một kernel thread và một periodic timer fire mỗi 1 giây. Timer callback in ra jiffies, thread in ra giá trị đếm.

## 📋 Yêu cầu

1. Spawn kernel thread với `kthread_run` (KHÔNG `kernel_thread`)
2. Thread kiểm tra `kthread_should_stop()` để thoát
3. Dùng `set_current_state(TASK_INTERRUPTIBLE)` + `schedule_timeout()` để sleep (không busy-wait)
4. Tạo periodic timer với `timer_setup` (KHÔNG `setup_timer`)
5. Timer callback dùng `from_timer` để lấy data
6. Timer re-arm với `mod_timer` mỗi 1 giây
7. Cleanup: `kthread_stop` và `del_timer_sync`

## ✅ Tiêu chí đạt

- Module load: thread bắt đầu chạy, timer bắt đầu fire
- `dmesg -w` mỗi 1 giây thấy "Timer fired" và "Thread iteration"
- `rmmod`: thread dừng, timer bị hủy, không có memory leak

## 💡 Gợi ý

- Timer callback chạy trong atomic context, không được gọi hàm sleep
- `from_timer(data, t, timer)` lấy con trỏ container chứa `struct timer_list`
- Thread callback chạy trong process context, có thể sleep
- `kthread_stop` block đến khi thread kết thúc, gọi trước `del_timer_sync`
- `jiffies + msecs_to_jiffies(1000)` tương đương 1 giây

## 🔍 Kiểm tra

```bash
make
sudo insmod thread_timer.ko
# Quan sát dmesg trong 5-10 giây
dmesg -w
sudo rmmod thread_timer
dmesg | tail
```

## 📁 File cần tạo

- [ ] `thread_timer.c` - Source code module
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-kernel-module.md`:
- Section 7.1: kthread_run / kthread_stop
- Section 7.2: kernel_timer with timer_setup

---

**Bonus:** Thêm sysfs attribute `/sys/kernel/kthread_timer/interval` để điều chỉnh interval timer (ms).
