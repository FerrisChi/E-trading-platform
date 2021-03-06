#ifndef UTILS_H
#define UTILS_H

#define MAX_MSG_LENGTH 1024

enum Opcode {
  RESET = 0,

  MODIFY_PASSWORD = 10,
  RECHARGE = 11,
  SET_DISCOUNT_INDEX = 12,
  SET_DISCOUNT_TYPE = 13,
  MODIFY_CHART = 14,
  MODIFY_PRODUCT = 15,
  CONSUME = 16,

  SEARCH_PRODUCT_DESC = 20,
  SEARCH_PRODUCT_ID = 21,
  GET_CHART = 22,
  LOGIN = 23,

  ADD_TO_CHART = 30,
  ADD_PRODUCT = 31,
  ADD_ORDER = 32,
  ADD_USER = 33,

  PAY_ORDER = 4,

  FREE_ORDER = 5,

  TEST = 6,

  CONNECT = 7
};

#endif  // UTILS_H
