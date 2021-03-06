/* Copyright (c) 2007-2019. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_KERNEL_ACTIVITY_ACTIVITYIMPL_HPP
#define SIMGRID_KERNEL_ACTIVITY_ACTIVITYIMPL_HPP

#include <string>
#include <list>

#include <xbt/base.h>
#include "simgrid/forward.h"

#include <atomic>
#include <simgrid/kernel/resource/Action.hpp>
#include <simgrid/simix.hpp>

namespace simgrid {
namespace kernel {
namespace activity {

class XBT_PUBLIC ActivityImpl {
  std::atomic_int_fast32_t refcount_{0};
public:
  virtual ~ActivityImpl();
  ActivityImpl() = default;
  e_smx_state_t state_ = SIMIX_WAITING; /* State of the activity */
  std::list<smx_simcall_t> simcalls_;   /* List of simcalls waiting for this activity */
  resource::Action* surf_action_ = nullptr;

  virtual void suspend();
  virtual void resume();
  virtual void cancel();
  virtual void post()   = 0; // What to do when a simcall terminates
  virtual void finish() = 0;

  virtual void register_simcall(smx_simcall_t simcall);
  void clean_action();
  virtual double get_remaining() const;
  // boost::intrusive_ptr<ActivityImpl> support:
  friend XBT_PUBLIC void intrusive_ptr_add_ref(ActivityImpl* activity);
  friend XBT_PUBLIC void intrusive_ptr_release(ActivityImpl* activity);

  static xbt::signal<void(ActivityImpl const&)> on_suspended;
  static xbt::signal<void(ActivityImpl const&)> on_resumed;
};

template <class AnyActivityImpl> class ActivityImpl_T : public ActivityImpl {
private:
  std::string name_             = "";
  std::string tracing_category_ = "";

public:
  AnyActivityImpl& set_name(const std::string& name)
  {
    name_ = name;
    return static_cast<AnyActivityImpl&>(*this);
  }
  const std::string& get_name() { return name_; }
  const char* get_cname() { return name_.c_str(); }

  AnyActivityImpl& set_tracing_category(const std::string& category)
  {
    tracing_category_ = category;
    return static_cast<AnyActivityImpl&>(*this);
  }
  const std::string& get_tracing_category() { return tracing_category_; }
};

} // namespace activity
} // namespace kernel
} // namespace simgrid

#endif /* SIMGRID_KERNEL_ACTIVITY_ACTIVITYIMPL_HPP */
