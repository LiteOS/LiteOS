#ifndef _TEST_OBJECT_ACCESS_CONTROL_H_
#define _TEST_OBJECT_ACCESS_CONTROL_H_
#include "liblwm2m.h"
#include <cpptest.h>
#include "stub.h"

extern "C" {
#include "object_comm.h"

}

class TestObjectAccessControl:public Test::Suite {
protected:
  void tear_down();
  void setup();
  
public:
  void test_acc_ctrl_create_object();
  void test_acl_ctrl_free_object();
  void test_acc_ctrl_obj_add_inst();
  void test_acc_ctrl_oi_add_ac_val();
  void test_acc_auth_operate();
  void test_prv_read();
  void test_prv_write();
  void test_prv_delete();
  void test_prv_create();


  
  TestObjectAccessControl();
};



#endif 


