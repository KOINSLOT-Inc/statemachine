// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "StateMachine.hpp"
#include <cstdio>
#include <cstdlib>

using namespace StateMachine;

typedef enum : uint16_t {
  EVENT_REQUEST_OCCUPATION = EVENT_USER,
  EVENT_REQUEST_LIFESTYLE,
  EVENT_REQUEST_TYPE,
  EVENT_TRANSFORMATIVE_MOMENT,
  EVENT_FINANCIAL_SUCCESS,
} Events;

void printEventName(Event event) {
  switch (event.signal) {
  case EVENT_REQUEST_PARENT:
    printf("(EVENT_REQUEST_PARENT)");
    break;
  case EVENT_REQUEST_CHILD:
    printf("(EVENT_REQUEST_CHILD)");
    break;
  case EVENT_ENTER:
    printf("(EVENT_ENTER)");
    break;
  case EVENT_EXIT:
    printf("(EVENT_EXIT)");
    break;
  case EVENT_REQUEST_OCCUPATION:
    printf("(EVENT_REQUEST_OCCUPATION)");
    break;
  case EVENT_REQUEST_LIFESTYLE:
    printf("(EVENT_REQUEST_LIFESTYLE)");
    break;
  case EVENT_REQUEST_TYPE:
    printf("(EVENT_REQUEST_TYPE)");
    break;
  case EVENT_TRANSFORMATIVE_MOMENT:
    printf("(EVENT_TRANSFORMATIVE_MOMENT)");
    break;
  case EVENT_FINANCIAL_SUCCESS:
    printf("(EVENT_FINANCIAL_SUCCESS)");
    break;
  };
}

class PersonMachine : public ::StateMachine::StateMachine {
public:
  void setup() { initialize((State)&PersonMachine::Farmer); };

  Result Person(Event event) {
    printf("    Person ");
    printEventName(event);
    printf(": ");
    switch (event.signal) {
    case EVENT_REQUEST_PARENT:
      printf("I have parents, but no parent state!\n");
      return Result{
          .type = RESULT_PARENT,
          .target = nullptr,
      };
      break;
    case EVENT_REQUEST_TYPE:
      printf("I'm a person!\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    default:
      printf("Hmm... I don't know what this event is.\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    }
  };

  Result Worker(Event event) {
    printf("    Worker ");
    printEventName(event);
    printf(": ");
    switch (event.signal) {
    case EVENT_REQUEST_PARENT:
      printf("My parent type is Person\n");
      return Result{
          .type = RESULT_PARENT,
          .target = (State)&PersonMachine::Person,
      };
      break;
    case EVENT_ENTER:
      printf("Time to get a job.\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    case EVENT_REQUEST_LIFESTYLE:
      printf("I'm a worker!\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    case EVENT_TRANSFORMATIVE_MOMENT:
      printf("I feel the call to adventure!\n");
      return Result{
          .type = RESULT_TRANSITION,
          .target = (State)&PersonMachine::Adventurer,
      };
      break;
    default:
      printf("Hmm... I don't know what this event is.\n");
      return Result{
          .type = RESULT_UNHANDLED,
          .target = nullptr,
      };
      break;
    }
  };

  Result Adventurer(Event event) {
    printf("    Adventurer ");
    printEventName(event);
    printf(": ");
    switch (event.signal) {
    case EVENT_REQUEST_PARENT:
      printf("My parent type is Person\n");
      return Result{
          .type = RESULT_PARENT,
          .target = (State)&PersonMachine::Person,
      };
      break;
    case EVENT_ENTER:
      printf("Let's go find a quest!\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    case EVENT_REQUEST_LIFESTYLE:
    case EVENT_REQUEST_OCCUPATION:
      printf("I'm an adventurer!\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    case EVENT_TRANSFORMATIVE_MOMENT:
      printf("Oof... I need to retire from adventuring...\n");
      return Result{
          .type = RESULT_TRANSITION,
          .target = (State)&PersonMachine::Farmer,
      };
      break;
    default:
      printf("Hmm... I don't know what this event is.\n");
      return Result{
          .type = RESULT_UNHANDLED,
          .target = nullptr,
      };
      break;
    }
  };

  Result Farmer(Event event) {
    printf("    Farmer ");
    printEventName(event);
    printf(": ");
    switch (event.signal) {
    case EVENT_REQUEST_PARENT:
      printf("My parent type is Worker\n");
      return Result{
          .type = RESULT_PARENT,
          .target = (State)&PersonMachine::Worker,
      };
      break;
    case EVENT_ENTER:
      printf("It's almost harvest season.\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    case EVENT_REQUEST_OCCUPATION:
      printf("I'm a farmer!\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    case EVENT_FINANCIAL_SUCCESS:
      printf("I can become a merchant!\n");
      return Result{
          .type = RESULT_TRANSITION,
          .target = (State)&PersonMachine::Merchant,
      };
      break;
    default:
      printf("Hmm... I don't know what this event is.\n");
      return Result{
          .type = RESULT_UNHANDLED,
          .target = nullptr,
      };
      break;
    }
  };

  Result Merchant(Event event) {
    printf("    Merchant ");
    printEventName(event);
    printf(": ");
    switch (event.signal) {
    case EVENT_REQUEST_PARENT:
      printf("My parent type is Worker\n");
      return Result{
          .type = RESULT_PARENT,
          .target = (State)&PersonMachine::Worker,
      };
      break;
    case EVENT_ENTER:
      printf("I have some shipments to fulfill.\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    case EVENT_REQUEST_OCCUPATION:
      printf("I'm a merchant!\n");
      return Result{
          .type = RESULT_HANDLED,
          .target = nullptr,
      };
      break;
    default:
      printf("Hmm... I don't know what this event is.\n");
      return Result{
          .type = RESULT_UNHANDLED,
          .target = nullptr,
      };
      break;
    }
  };

} personMachine;

extern "C" void app_main(void) {
  printf("Setting up state machine:\n");
  personMachine.setup();
  printf("\n");

  printf("Requesting info about current state:\n");
  personMachine.handle({.signal = EVENT_REQUEST_TYPE});
  personMachine.handle({.signal = EVENT_REQUEST_LIFESTYLE});
  personMachine.handle({.signal = EVENT_REQUEST_OCCUPATION});
  printf("\n");

  printf("Sending a transformative moment event:\n");
  personMachine.handle({.signal = EVENT_TRANSFORMATIVE_MOMENT});
  printf("\n");

  printf("Requesting info about current state:\n");
  personMachine.handle({.signal = EVENT_REQUEST_TYPE});
  personMachine.handle({.signal = EVENT_REQUEST_LIFESTYLE});
  personMachine.handle({.signal = EVENT_REQUEST_OCCUPATION});
  printf("\n");

  printf("Sending a transformative moment event:\n");
  personMachine.handle({.signal = EVENT_TRANSFORMATIVE_MOMENT});
  printf("\n");

  printf("Requesting info about current state:\n");
  personMachine.handle({.signal = EVENT_REQUEST_TYPE});
  personMachine.handle({.signal = EVENT_REQUEST_LIFESTYLE});
  personMachine.handle({.signal = EVENT_REQUEST_OCCUPATION});
  printf("\n");

  printf("Sending a financial success event:\n");
  personMachine.handle({.signal = EVENT_FINANCIAL_SUCCESS});
  printf("\n");

  printf("Requesting info about current state:\n");
  personMachine.handle({.signal = EVENT_REQUEST_TYPE});
  personMachine.handle({.signal = EVENT_REQUEST_LIFESTYLE});
  personMachine.handle({.signal = EVENT_REQUEST_OCCUPATION});
  printf("\n");

  exit(0);
}
