!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Wrapper for array
module saydx_array
  use saydx_cinterop, only : c_null_ptr, c_associated
  use saydx_ciface, only : c_array_dereference
  use saydx_commontypes, only : c_ptr_wrapper_t
  implicit none
  private

  public :: array_t


  type, extends(c_ptr_wrapper_t) :: array_t
  contains
    final :: array_final
  end type array_t


contains

  subroutine array_final(this)
    type(array_t), intent(inout) :: this

    if (.not. c_associated(this%cptr)) then
      return
    end if
    call c_array_dereference(this%cptr)
    this%cptr = c_null_ptr
  end subroutine array_final


end module saydx_array
