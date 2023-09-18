#ifndef RWLATCH_H
#define RWLATCH_H
#include <shared_mutex>

// 读写锁
class RWLatch {
 public:
  /**
   * Acquire a write latch.
   */
  void w_lock() { mutex_.lock(); }

  /**
   * Release a write latch.
   */
  void w_unlock() { mutex_.unlock(); }

  /**
   * Acquire a read latch.
   */
  void r_lock() { mutex_.lock_shared(); }

  /**
   * Release a read latch.
   */
  void r_unlock() { mutex_.unlock_shared(); }

 private:
  std::shared_mutex mutex_;
};
#endif  // RWLATCH_H