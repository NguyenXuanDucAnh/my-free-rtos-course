# Description

### Task 1, 2, 3 (Sensor Tasks):

- nĐọc dữ liệu từ cảm biến (giả lập bằng vTaskDelay)

- Đồng bộ bằng xEventGroupSync() để báo đã hoàn thành vòng xử lý

### Task 4 (Processing Task):

- Chờ cả 3 task cảm biến hoàn thành

- Xử lý dữ liệu tổng hợp sau khi tất cả sẵn sàng