# Description

## Tuy nhiên, các hàm API xEventGroupSetBits() và xEventGroupWaitBits() không thể được sử dụng trong tình huống đồng bộ hóa này. Nếu sử dụng, thì việc thiết lập một bit (để báo hiệu một task đã đến điểm đồng bộ) và việc kiểm tra các bit (để xác định các task khác đã đến điểm đồng bộ chưa) sẽ được thực hiện như hai thao tác riêng biệt.

## Để thấy tại sao điều đó là vấn đề, hãy xét một kịch bản trong đó Task A, Task B và Task C cố gắng đồng bộ hóa bằng một Event Group:

- Task A và Task B đã đến điểm đồng bộ (sync point), vì vậy bit của chúng đã được set trong event group, và chúng đang ở trạng thái Blocked để chờ bit của Task C cũng được set.

- Task C đến điểm đồng bộ sau cùng và sử dụng xEventGroupSetBits() để set bit của nó trong event group. Ngay khi bit của Task C được set, Task A và Task B sẽ thoát khỏi trạng thái Blocked và xóa tất cả các bit trong event group.

- Sau đó, Task C gọi xEventGroupWaitBits() để chờ cả 3 bit được set, nhưng vào thời điểm đó, cả 3 bit đã bị xóa mất rồi, Task A và Task B đã rời khỏi điểm đồng bộ, và vì vậy quá trình đồng bộ hóa đã thất bại. task C bị dead ở wait bit. Task A và task B sau khi thoát được wait bit thì thực hiện vòng lặp mới tuy nhiên cũng bị dead ở wait bit do không còn task nào set bit được nữa => hiện tượng deadlock xảy ra và chương trình không còn hoạt động nữa.