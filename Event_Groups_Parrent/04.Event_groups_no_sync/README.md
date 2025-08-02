# Description

### bài toán: thu thập đủ các dữ liệu tại cùng thời điểm sau đó post bản tin lên server. Dữ liệu sẽ được sử dụng cho training model sau này nên cần dữ liệu đúng tại thời điểm thu.

### Task 1, 2, 3 (Sensor Tasks):

- nĐọc dữ liệu từ cảm biến (giả lập bằng vTaskDelay)

- Đồng bộ bằng xEventGroupSync() để báo đã hoàn thành vòng xử lý

### Task 4 (Processing Task):

- Chờ cả 3 task cảm biến hoàn thành

- Xử lý dữ liệu tổng hợp sau khi tất cả đã set bit

# tuy nhiên có một vấn đề đó là các task này không đồng bộ. có task đã chạy và thu rất nhiều lần dữ liệu rồi nhưng có task lại chưa được chạy lần nào