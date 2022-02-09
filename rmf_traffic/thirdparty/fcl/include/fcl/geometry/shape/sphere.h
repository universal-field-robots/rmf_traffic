/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011-2014, Willow Garage, Inc.
 *  Copyright (c) 2014-2016, Open Source Robotics Foundation
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Open Source Robotics Foundation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/** @author Jia Pan */

#ifndef FCL_SHAPE_SPHERE_H
#define FCL_SHAPE_SPHERE_H

#include "fcl/geometry/shape/shape_base.h"

#include <iostream>
#include <atomic>

namespace fcl
{

/// @brief Center at zero point sphere
template <typename S_>
class FCL_EXPORT Sphere : public ShapeBase<S_>
{
public:

  using S = S_;

  Sphere(S radius);

  /// @brief Radius of the sphere
  S radius;

  /// @brief Compute AABB<S>
  void computeLocalAABB() override;

  /// @brief Get node type: a sphere
  NODE_TYPE getNodeType() const override;

  Matrix3<S> computeMomentofInertia() const override;

  S computeVolume() const override;

  /// @brief get the vertices of some convex shape which can bound this shape in
  /// a specific configuration
  std::vector<Vector3<S>> getBoundVertices(const Transform3<S>& tf) const;

  friend
  std::ostream& operator<<(std::ostream& out, const Sphere& sphere) {
    out << "Sphere(" << sphere.radius << ")";
    return out;
  }

  /// An efficient spin lock to ensure that threads only spend a minimal amount
  /// of time trying to obtain this lock. The implementation is inspired by
  /// https://rigtorp.se/spinlock/
  class SpinLock
  {
  public:
    SpinLock(std::atomic_bool& mutex)
    : _mutex(mutex)
    {
      // When the exchange produces a false value, we will know that we have
      // obtained "ownership" of the mutex.
      while (_mutex.exchange(true, std::memory_order_acquire));
    }

    ~SpinLock()
    {
      _mutex.store(false, std::memory_order_release);
    }

  private:
    std::atomic_bool& _mutex;
  };

private:
  std::atomic_bool _mutex_compute_local_aabb = {false};
  std::optional<S> _last_local_aabb_radius = std::nullopt;
};

using Spheref = Sphere<float>;
using Sphered = Sphere<double>;

} // namespace fcl

#include "fcl/geometry/shape/sphere-inl.h"

#endif