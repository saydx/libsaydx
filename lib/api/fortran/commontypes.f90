!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Commonly used type
module saydx_commontypes
  use saydx_common, only : fatal_error
  use saydx_cinterop, only : c_ptr, c_null_ptr, c_associated
  implicit none
  private

  public :: c_ptr_wrapper_t, error_t
  public :: handle_error


  type :: c_ptr_wrapper_t
    type(c_ptr) :: cptr = c_null_ptr
  contains
    procedure :: is_associated => c_ptr_wrapper_is_associated
    procedure, private :: c_ptr_wrapper_assign
    generic :: assignment(=) => c_ptr_wrapper_assign
    procedure :: move_to => c_ptr_wrapper_move_to
  end type c_ptr_wrapper_t


  type, extends(c_ptr_wrapper_t) :: error_t
    private
  end type error_t


contains

  function c_ptr_wrapper_is_associated(this) result(isassoc)
    class(c_ptr_wrapper_t), intent(in) :: this
    logical :: isassoc

    isassoc = c_associated(this%cptr)

  end function c_ptr_wrapper_is_associated


  subroutine c_ptr_wrapper_assign(lhs, rhs)
    class(c_ptr_wrapper_t), intent(inout) :: lhs
    class(c_ptr_wrapper_t), intent(in) :: rhs

    call fatal_error("Assignment of c pointer wrappers is not permitted")

  end subroutine c_ptr_wrapper_assign


  subroutine c_ptr_wrapper_move_to(this, newthis)
    class(c_ptr_wrapper_t), intent(inout) :: this
    class(c_ptr_wrapper_t), intent(out) :: newthis

    newthis%cptr = this%cptr
    this%cptr = c_null_ptr

  end subroutine c_ptr_wrapper_move_to


  subroutine handle_error(error, msg, opterror)
    type(error_t), intent(inout) :: error
    character(*), intent(in) :: msg
    type(error_t), intent(out), optional :: opterror

    if (present(opterror)) then
      call error%move_to(opterror)
      return
    end if
    if (error%is_associated()) then
      call fatal_error(msg)
    end if

  end subroutine handle_error


end module saydx_commontypes
