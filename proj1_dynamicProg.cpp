#include <iostream>
#include <list>
#include <vector>
#include <algorithm>                  
#include <chrono>

auto start_time = std::chrono::high_resolution_clock::now();


int main(){
    int initLen, initWidth, n, minWidth = 0;
    if(scanf("%d %d %d", &initLen, &initWidth, &n) != 3)
        return 1;
    if(initWidth > initLen)
        std::swap(initWidth, initLen);
    
    std::vector<std::vector<int>> matrix(initWidth + 1, std::vector<int>(initLen + 1));

    for(int i = 0; i < n; i++){
        int plate_len, plate_width, plate_price; 
        if(scanf("%d %d %d", &plate_len, &plate_width, &plate_price) != 3)
            return 1;

        if(plate_width > plate_len)
            std::swap(plate_width, plate_len);
        
        if( minWidth == 0 && plate_width <= initWidth && plate_len <= initLen)
            minWidth = plate_width;
    
        else
            minWidth = std::min(minWidth,plate_width);
            
        if (plate_width <= initWidth && plate_len <= initLen) {
            matrix[plate_width][plate_len] = std::max(matrix[plate_width][plate_len], plate_price);

            if (plate_len <= initWidth && plate_width <= initLen)
                matrix[plate_len][plate_width] = matrix[plate_width][plate_len];
        }
    }
    if(minWidth == 0){
        std::cout << 0 << std::endl;
        return 0;
    }
    for(int i = minWidth; i <= initWidth; i++){
        for(int j = i; j <= initLen; j++){
            for(int k = 1; k <= j/2; k++){
                matrix[i][j] = std::max(matrix[i][j], matrix[i][k] + matrix[i][j-k]);
                if(k <= i/2)
                    matrix[i][j] = std::max(matrix[i][j], matrix[k][j] + matrix[i-k][j]);
            }
            if(j <= initWidth && i <= initLen)
                    matrix[j][i] = matrix[i][j];                  
        }
    }

    std::cout << matrix[initWidth][initLen] << std::endl;
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Tempo de execução: " << duration.count() << " microssegundos" << std::endl;
    return 0;
}