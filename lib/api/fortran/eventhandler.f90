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

  public :: eventhandler_t, eventhandler_final


  type, extends(c_ptr_wrapper_t) :: eventhandler_t
  contains
    final :: eventhandler_final
  end type eventhandler_t


contains


  subroutine eventhandler_final(this)
    type(eventhandler_t), intent(inout) :: this

    if (.not. c_associated(this%cptr)) then
      return
    end if
    call c_eventhandler_destroy(this%cptr);
    this%cptr = c_null_ptr

  end subroutine eventhandler_final


end module saydx_eventhandler
