#include <stdio.h>

int main() {
    float length, width, area;
    
    printf("=== CHƯƠNG TRÌNH TÍNH DIỆN TÍCH HÌNH CHỮ NHẬT ===\n");
    printf("Nhập chiều dài: ");
    scanf("%f", &length);
    
    printf("Nhập chiều rộng: ");
    scanf("%f", &width);
    
    area = length * width;
    
    printf("\nDiện tích hình chữ nhật: %.2f\n", area);
    
    return 0;
}
