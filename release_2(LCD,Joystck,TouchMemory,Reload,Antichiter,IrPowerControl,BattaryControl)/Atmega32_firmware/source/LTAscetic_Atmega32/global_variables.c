/**************************************************************************************
* ���������� ����������
***************************************************************************************/
#include <avr/io.h>        // ���������� ����� ��� ������ �����-�����
#include <util/delay.h>    // ���� ����������� ������������ ��������
#include <avr/interrupt.h> // ����� ������������ ����������
#include <avr/pgmspace.h>  //����� ������� ��������� � ������ ��������
#include <avr/eeprom.h>
#include "types.h"
#include "miles_protocol.h"
volatile uint16_t timer1; 


volatile bool start_bit_received;				//���� ���� ��������������, ���� ������ �����-���

volatile uint16_t high_level_counter; 			//������� ������������ ������� �������� ������ �� ������ ��-���������										
												//������� ����� �������� ���������


volatile uint16_t bit_in_rx_buff; 				//���������� ���, �������� ��-���������� 

volatile trx_event rx_event; 					//������� ��-���������

volatile uint16_t low_level_counter; 			//������� ������������ ������� ������� ������ �� ������ ��-���������										
												//������� ����� �������� ���������


volatile uint8_t rx_buffer[RX_BUFFER_SIZE]; 	//������ ��-���������

volatile uint8_t tx_buffer[TX_BUFFER_SIZE]; 	//������ ��-���������


volatile bool ir_transmitter_on;	//����, ����������� (true) ��� ����������� �������� ������ ����� ��-���� 

volatile int ir_pulse_counter; 					//�������� ������� "�������" ��-�����

volatile int ir_space_counter; 					//�������� ������� ������������ ������������ ��������� ��-����� (����� ����� ������) 

volatile union data_packet_union  data_packet; 	//� ���� ���������� ����� ����������� ����� ������ ��� �������� ����� IR


volatile uint8_t cursor_position;				//��� ���������� ����� ������� ����� �������� ������� ������ data_packet.data[x]

volatile union data_packet_union  data_packet; 	//� ���� ���������� ����� ����������� ����� ������ ��� �������� ����� IR

volatile trx_packet rx_packet;

volatile uint8_t life; //������� ����� (��������)

volatile uint8_t life_leds_status[4]; //���� ������ ����� ������� ��������� 

volatile TKEYBOARD_EVENT  keyboard_event; //������� ���������� 

volatile TKEYBOARD_EVENT  reload_key_event; //������� ������� "������������"

volatile TJOYSTICK_EVENT  joystick_event; //������� ��������� 

volatile struct pressing_duration key_pressing_duration;//��������� ������ �������� ������������ ������� ������

volatile struct joystick_pressing_duration joystick_key_pressing_duration;

volatile uint8_t fire_led_status; //������ ���������� ������� �������� 

volatile uint8_t bullets;  //���������� �������� � ��������

volatile uint16_t last_simple;					//���������� ����� ��������� ������� �����

volatile bool play_hit_snd; //���� ���� ��������� (true) ��� ��������� (false) ��������������� �������������� ������

volatile tsnd_adress snd_adress; //� ���� ���������� ����� ������� ����� ��������� �������, ������� ����� �������������


volatile EEMEM tteam_color eeprom_team_id;
//volatile uint8_t shift_register_buffer; //� ���� ���������� ����� ������� ������ ��� ���������� �������� ����� ���������
volatile EEMEM uint8_t eeprom_player_id;

volatile EEMEM tgun_damage eeprom_damage;

volatile EEMEM uint8_t eeprom_bullets_in_clip; // ���������� �������� � ������

volatile EEMEM uint8_t eeprom_clips; // ���������� ����� ��� �������������

volatile EEMEM uint8_t eeprom_reload_duration; // ������������ ����������� (� ��������)

volatile EEMEM uint16_t eeprom_batt_full_voltage; // ���������� ��������� ���������� ������� (�������� ���)

volatile EEMEM uint16_t eeprom_batt_low_voltage; // ���������� ��������� ����������� ������� (�������� ���)

volatile EEMEM uint8_t eeprom_curr_ir_pin; //���������� �������� �� ���������

volatile EEMEM ttm_serial_num eeprom_tm_serial_num;

volatile uint8_t clips;//���-�� ���������� �����

volatile life_in_percent;// ������� "�����" � ��������

volatile uint16_t chit_detected_counter; // ������� ������������ ���������� �������

volatile bool chit_detected; // ����, ������� �������� true, ���� ������������� ���������� �������

volatile bool tm_connect; //����, ������� �������� true, ���� ������������� ���������� ����� ��������� � �����������


volatile bool display_bullets_update_now; //����, �����������, ��� ���� �������� ��������� ���������� ��������
//volatile TTM_EVENT tm_event; //������� ����������� TouchMemory



volatile TDISPLAY_BATT_MODE display_batt_mode; //����� ����������� ���������� ������� (������/�����) 


volatile uint8_t curr_ir_pin; //������� �����, ������� ����� "�������" ��� ��������, ���������� �������� �� ���������


//������ ��� ��������� ��������� �������� �����. �������� ���� = damage_value[Damage_xx]
uint8_t damage_value [] PROGMEM = 
{
	1,   // 0000 = 1   (Damage_1)
	2,   // 0001 = 2   (Damage_2)
	4,   // 0010 = 4   (Damage_4)
	5,   // 0011 = 5   (Damage_5)
	7,   // 0100 = 7   (Damage_7)
	10,  // 0101 = 10  (Damage_10)
	15,  // 0110 = 15  (Damage_15)
	17,  // 0111 = 17  (Damage_17)
	20,  // 1000 = 20  (Damage_20)
	25,  // 1001 = 25  (Damage_25)
	30,  // 1010 = 30  (Damage_30)
	35,  // 1011 = 35  (Damage_35)
	40,  // 1100 = 40  (Damage_40)
	50,  // 1101 = 50  (Damage_50)
	75,  // 1110 = 75  (Damage_75)
	100, // 1111 = 100 (Damage_100)

};
