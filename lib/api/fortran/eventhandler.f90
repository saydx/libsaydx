!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Wrapper for eventhandler
module saydx_eventhandler
  use saydx_cinterop, only : c_null_ptr, c_associated
  use saydx_ciface, only : c_eventhandler_destroy
  use saydx_commontypes
  implicit none
  private

  public :: eventhandler_t, eventhandler_finalize


  type, extends(c_ptr_wrapper_t) :: eventhandler_t
  contains
    final :: eventhandler_finalize
  end type eventhandler_t


contains


  subroutine eventhandler_finalize(this)
    type(eventhandler_t), intent(inout) :: this

    if (c_associated(this%cptr)) then
      call c_eventhandler_destroy(this%cptr);
    end if
    this%cptr = c_null_ptr

  end subroutine eventhandler_finalize


end module saydx_eventhandler
