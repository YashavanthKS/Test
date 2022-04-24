#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define PREFIX_LEN 16
#define TOTAL_MASK 32

int powof(int val, int count){

   int res = 1;
   while (count != 0){
       res *= val;
       --count;
   }
   return res;
}
void
parseIPV4string(char* ipAddress, int *int_arr) {
    char ipbytes[4];
    int i,j = 0;
    int k = 0;
    int count = 0;
    int octet[PREFIX_LEN - 3];
    for (i = PREFIX_LEN; i >= 0 ; i--){
        if(ipAddress[i] != '.'){
            if((ipAddress[i] >= '0') && (ipAddress[i] <= '9')){
                octet[j] = ipAddress[i] - '0';
                j++;
                count++;
            }
        }
        else{
            count = count -1;
            int_arr[k] = 0;
            while(count >= 0){
                int_arr[k] += powof(10, count) * octet[count];
                --count;
            }

            k++;
            j = 0;
            count = 0;
            memset(octet, 0, PREFIX_LEN - 3);
        }
    }
    count = count -1;
    int_arr[k] = 0;
    while(count >= 0){ 
        int_arr[k] += powof(10, count) * octet[count];
        --count;
    }
}

void
get_broadcast_address(char *ip_addr, int mask, char *output_buffer){
    int i = 0;
    int j = 0;
    int int_arr[PREFIX_LEN-3];
    unsigned int mask_bit = TOTAL_MASK - mask;
    unsigned int mask_val = 0;
    memset(int_arr, 0, PREFIX_LEN-3);
    parseIPV4string(ip_addr, int_arr);
    if(mask_bit <= 8){
        mask_val = powof(2, mask_bit);
        int_arr[i] = int_arr[i] | (mask_val - 1);
        i++;
    }
    else if(mask_bit > 8){
        while((mask_bit % 8) && (mask_bit != 0)){
            if(mask_bit < 8){
                mask_val = powof(2, mask_bit);
                int_arr[i] = int_arr[i] | (mask_val - 1);
                mask_bit = 0;
            }
            else{
                mask_val = powof(2, 8);
                int_arr[i] = int_arr[i] | (mask_val - 1);
                i++;
                mask_bit -= 8;
            }
        }
    }
            
    sprintf(output_buffer, "%d.%d.%d.%d", int_arr[3], int_arr[2], int_arr[1], int_arr[0]);
}

unsigned int
get_ip_integral_equivalent(char *ip_addr){
    int int_arr[PREFIX_LEN-3];
    memset(int_arr, 0, PREFIX_LEN-3);
    parseIPV4string(ip_addr, int_arr);
    return(int_arr[0] | (int_arr[1] << 8) | (int_arr[2] << 16) | (int_arr[3] << 24));
}

void
get_abcd_ip_format(unsigned int ip_address,
                     char *output_buffer)
{
    unsigned char bytes[4];
    bytes[0] = ip_address & 0xFF;
    bytes[1] = (ip_address >> 8) & 0xFF;
    bytes[2] = (ip_address >> 16) & 0xFF;
    bytes[3] = (ip_address >> 24) & 0xFF;   
    sprintf(output_buffer, "%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

void
get_network_id(char *ip_addr, int mask, char *output_buffer){
    int i = 0;
    int j = 0;
    int int_arr[PREFIX_LEN-3];
    unsigned int mask_bit = TOTAL_MASK - mask;
    unsigned int mask_val = 0;
    memset(int_arr, 0, PREFIX_LEN-3);
    parseIPV4string(ip_addr, int_arr);
    if(mask_bit <= 8){ 
        mask_val = powof(2, mask_bit);
        int_arr[i] = int_arr[i] & (~(mask_val - 1)); 
        i++;
    }   
    else if(mask_bit > 8){ 
        while((mask_bit % 8) && (mask_bit != 0)){
            if(mask_bit < 8){ 
                mask_val = powof(2, mask_bit);
                int_arr[i] = int_arr[i] & (~(mask_val - 1)); 
                mask_bit = 0;
            }
            else{
                mask_val = powof(2, 8);
                int_arr[i] = int_arr[i] & (~(mask_val - 1)); 
                i++;
                mask_bit -= 8;
            }
        }
    }   
     
    sprintf(output_buffer, "%d.%d.%d.%d", int_arr[3], int_arr[2], int_arr[1], int_arr[0]);
}

int get_subnet_cardinality(int mask){
    return ((powof(2, (TOTAL_MASK - mask))) - 2);
}

int check_ip_subnet_member_ship(char *network_id,
                                int mask,
                                char *check_ip){
    char output_buffer[PREFIX_LEN];
    get_network_id(check_ip, mask, output_buffer);
    return (strcmp(output_buffer, network_id));
}

int main(){
    char ipadd_buffer[PREFIX_LEN];
    memset(ipadd_buffer, 0, PREFIX_LEN);
    char ip_add[PREFIX_LEN];
    unsigned int int_ip;
    int mask = 32;
    int choice = 0;
    int res;
    printf("\n");
    printf("Enter 1 for broadcast address\n");
    printf("Enter 2 for IP integer equivalent\n");
    printf("Enter 3 for convert integer to IP\n");
    printf("Enter 4 for network id\n");
    printf("Enter 5 for subnet cardinality\n");
    printf("Enter 6 for subnet membership\n");
    scanf("%d", &choice);
    printf("\n");

    if(choice == 1){
        printf("Enter the ip ");
        scanf("%s", ip_add);
        printf("Enter the mask ");
        scanf("%d", &mask);
        printf("\n");
        get_broadcast_address(ip_add, mask, ipadd_buffer);
        printf("Broadcast address = %s\n", ipadd_buffer);
    }
    else if(choice == 2){
        printf("Enter the ip ");
        scanf("%s", ip_add);
        printf("IP integer equivalent = %u\n", get_ip_integral_equivalent(ip_add));
    }
    else if(choice == 3){
        printf("Enter the int ip ");
        scanf("%u", &int_ip);
        printf("int_ip %u\n", int_ip);
        get_abcd_ip_format(int_ip, ipadd_buffer);
        printf("IP address in A.B.C.D format = %s\n", ipadd_buffer);
    }
    else if(choice == 4){
        printf("Enter the ip ");
        scanf("%s", ip_add);
        printf("Enter the mask ");
        scanf("%d", &mask);
        printf("\n");
        get_network_id(ip_add, mask, ipadd_buffer);
        printf("Network Id = %s\n", ipadd_buffer);
    }
    else if(choice == 5){
        printf("Enter the mask ");
        scanf("%d", &mask);
        printf("Subnet cardinality for mask = %u is %u\n", mask, get_subnet_cardinality(mask));
    }
    else if(choice == 6){
        printf("Enter the network ip ");
        scanf("%s", ip_add);
        printf("Enter the mask ");
        scanf("%d", &mask);
        printf("Enter the ip to check ");
        scanf("%s", ipadd_buffer);
        res = check_ip_subnet_member_ship(ip_add, mask, ipadd_buffer);
        printf("IP Subnet check Result = %s\n", res == 0 ? "Membership true": "Membership false");
    }
}
