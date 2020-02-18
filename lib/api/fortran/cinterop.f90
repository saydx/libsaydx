!!>
!!> Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
!!> Use of this source code is governed by a BSD-style
!!> license that can be found in the LICENSE file.
!!>

module cinterop
  use, intrinsic :: iso_c_binding, only : c_char, c_ptr, c_size_t, c_f_pointer, c_null_char
  implicit none
  private

  public :: c_f_string_pointer, f_c_string


  interface
    function c_strlen(pstr) result(len) bind(C, name='strlen')
      import :: c_ptr, c_size_t
      type(c_ptr), value :: pstr
      integer(c_size_t) :: len
    end function c_strlen
  end interface

contains

  subroutine c_f_string_pointer(cptr, fptr)
    type(c_ptr), intent(in) :: cptr
    character(:), pointer, intent(out) :: fptr

    fptr => c_f_string_pointer_helper(cptr, c_strlen(cptr))

  end subroutine c_f_string_pointer


  function c_f_string_pointer_helper(cptr, len) result(fptr)
    type(c_ptr), intent(in) :: cptr
    integer(c_size_t), intent(in) :: len
    character(len), pointer :: fptr

    call c_f_pointer(cptr, fptr)

  end function c_f_string_pointer_helper


  function f_c_string(fstr) result(cstr)
    character(len=*, kind=c_char), intent(in) :: fstr
    character(len=:, kind=c_char), allocatable :: cstr

    cstr = trim(fstr) // c_null_char

  end function f_c_string

end module cinterop
