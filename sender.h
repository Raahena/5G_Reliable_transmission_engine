#ifndef SENDER_H
#define SENDER_H
#include <stdint.h>
#include "packet.h"
#include "window.h"
#include "timer.h"

/*
 * Send a single packet
 */
void sendPacket(Packet *packet);

/*
 * Retransmit a single packet
 */
void retransmitPacket(Packet *packet);

/*
 * Send all packets inside
 * current sender window
 */

void sendPacketsInWindow(
        Packet packets[],
        Window *window,
        int totalPackets,
        Timer *timer,
        int timeout);
/*
 * Go-Back-N Retransmission
 */
void goBackNRetransmit(
        Packet packets[],
        Window *window);

/*
 * Display Sender Window
 */
void displaySenderWindow(
        Packet packets[],
        Window *window);

#endif
