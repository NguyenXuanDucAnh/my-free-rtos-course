# Description

Sử dụng 2 task, 1 task mô phỏng thu dữ liệu từ cảm biến nhiệt độ, 1 task mô phỏng thu dữ liệu từ cảm biến độ ẩm.

Do xClearOnExit được set là pdFALSE nên chỉ được lần đầu tiên thì task process chờ 2 task cùng phát event, còn những lần sau do bit vẫn được set nguyên giá trị nên dù 2 task nhiệt độ và độ ẩm không set bit thì task process data vẫn đi vào thực thi chương trình (do bit vẫn được set và logic vẫn đúng)

==> tạo một task clear bit để mô phỏng trường hợp nhiều task nhận cùng 1 event group và phải có một task clear bit (thường là task nhận cuối cùng)
task clear bit này sẽ nhận semaphore từ task process data để biết rằng cần phải clear bit (hoặc dùng luôn event group cũng được)