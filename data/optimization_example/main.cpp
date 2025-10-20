

#include "receiver.h"
#include "sender.h"

extern Sender sender_0;
extern Sender sender_1;
extern Sender sender_2;
extern Sender sender_3;
extern Sender sender_4;
extern Sender sender_5;
extern Sender sender_6;
extern Sender sender_7;
extern Sender sender_8;
extern Sender sender_9;
extern Sender sender_10;
extern Sender sender_11;
extern Sender sender_12;
extern Sender sender_13;
extern Sender sender_14;
extern Sender sender_15;
extern Sender sender_16;
extern Sender sender_17;
extern Sender sender_18;
extern Sender sender_19;
extern Sender sender_20;
extern Sender sender_21;
extern Sender sender_22;
extern Sender sender_23;
extern Sender sender_24;
extern Sender sender_25;
extern Sender sender_26;
extern Sender sender_27;
extern Sender sender_28;
extern Sender sender_29;
extern Sender sender_30;

extern Receiver receiver_0;
extern Receiver receiver_1;
extern Receiver receiver_2;
extern Receiver receiver_3;
extern Receiver receiver_4;
extern Receiver receiver_5;
extern Receiver receiver_6;
extern Receiver receiver_7;
extern Receiver receiver_8;
extern Receiver receiver_9;
extern Receiver receiver_10;
extern Receiver receiver_11;
extern Receiver receiver_12;
extern Receiver receiver_13;
extern Receiver receiver_14;
extern Receiver receiver_15;
extern Receiver receiver_16;
extern Receiver receiver_17;
extern Receiver receiver_18;
extern Receiver receiver_19;
extern Receiver receiver_20;
extern Receiver receiver_21;
extern Receiver receiver_22;
extern Receiver receiver_23;
extern Receiver receiver_24;
extern Receiver receiver_25;
extern Receiver receiver_26;
extern Receiver receiver_27;
extern Receiver receiver_28;
extern Receiver receiver_29;
extern Receiver receiver_30;

// static Sender *senders[] = {
//         &sender_0,
//         &sender_1,
//         &sender_2,
//         &sender_3,
//         &sender_4,
//         &sender_5,
//         &sender_6,
//         &sender_7,
//         &sender_8,
//         &sender_9,
//         &sender_10,
//         &sender_11,
//         &sender_12,
//         &sender_13,
//         &sender_14,
//         &sender_15,
//         &sender_16,
//         &sender_17,
//         &sender_18,
//         &sender_19,
//         &sender_20,
//         &sender_21,
//         &sender_22,
//         &sender_23,
//         &sender_24,
//         &sender_25,
//         &sender_26,
//         &sender_27,
//         &sender_28,
//         &sender_29,
//         &sender_30};

// static Receiver *receivers[] = {
//         &receiver_0,
//         &receiver_1,
//         &receiver_2,
//         &receiver_3,
//         &receiver_4,
//         &receiver_5,
//         &receiver_6,
//         &receiver_7,
//         &receiver_8,
//         &receiver_9,
//         &receiver_10,
//         &receiver_11,
//         &receiver_12,
//         &receiver_13,
//         &receiver_14,
//         &receiver_15,
//         &receiver_16,
//         &receiver_17,
//         &receiver_18,
//         &receiver_19,
//         &receiver_20,
//         &receiver_21,
//         &receiver_22,
//         &receiver_23,
//         &receiver_24,
//         &receiver_25,
//         &receiver_26,
//         &receiver_27,
//         &receiver_28,
//         &receiver_29,
//         &receiver_30};

int main() {
    sender_0.connect(&receiver_0);
    sender_1.connect(&receiver_1);
    sender_2.connect(&receiver_2);
    sender_3.connect(&receiver_3);
    sender_4.connect(&receiver_4);
    sender_5.connect(&receiver_5);
    sender_6.connect(&receiver_6);
    sender_7.connect(&receiver_7);
    sender_8.connect(&receiver_8);
    sender_9.connect(&receiver_9);
    sender_10.connect(&receiver_10);
    sender_11.connect(&receiver_11);
    sender_12.connect(&receiver_12);
    sender_13.connect(&receiver_13);
    sender_14.connect(&receiver_14);
    sender_15.connect(&receiver_15);
    sender_16.connect(&receiver_16);
    sender_17.connect(&receiver_17);
    sender_18.connect(&receiver_18);
    sender_19.connect(&receiver_19);
    sender_20.connect(&receiver_20);
    sender_21.connect(&receiver_21);
    sender_22.connect(&receiver_22);
    sender_23.connect(&receiver_23);
    sender_24.connect(&receiver_24);
    sender_25.connect(&receiver_25);
    sender_26.connect(&receiver_26);
    sender_27.connect(&receiver_27);
    sender_28.connect(&receiver_28);
    sender_29.connect(&receiver_29);
    sender_30.connect(&receiver_30);

    // for (int i = 0; i < 31; ++i)
    // {
    //     senders[i]->connect(receivers[i]);
    // }
}
