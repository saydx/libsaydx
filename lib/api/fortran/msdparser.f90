!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Wrapper for the MSD parser.
module saydx_msdparser
  use saydx_cinterop, only : c_null_ptr, f_c_string
  use saydx_ciface, only : c_msdparser_create, c_msdparser_destroy, c_msdparser_parse_file
  use saydx_common, only : stderr
  use saydx_commontypes, only : c_ptr_wrapper_t, error_t
  use saydx_eventhandler
  implicit none
  private

  public :: msdparser_t, msdparser_init

  type, extends(c_ptr_wrapper_t) :: msdparser_t
  contains
    private
    procedure, public :: parse_file => msdparser_parse_file
    final :: msdparser_finalize
  end type msdparser_t

contains

  subroutine msdparser_init(this, eventhandler)
    type(msdparser_t), intent(out) :: this
    class(eventhandler_t), intent(in) :: eventhandler

    this%cptr = c_msdparser_create(eventhandler%cptr)

  end subroutine msdparser_init


  subroutine msdparser_parse_file(this, fname, error)
    class(msdparser_t), intent(inout) :: this
    character(*), intent(in) :: fname
    type(error_t), intent(out), optional :: error

    type(error_t) :: error_

    error_%cptr = c_msdparser_parse_file(this%cptr, f_c_string(fname))
    if (present(error)) then
      error = error_
      return
    elseif (error%is_associated()) then
      write(stderr, "(A)") "Unhandler error in 'msdparser_parse_file'"
      error stop
    end if

  end subroutine msdparser_parse_file


  subroutine msdparser_finalize(this)
    type(msdparser_t), intent(inout) :: this

    call c_msdparser_destroy(this%cptr)
    this%cptr = c_null_ptr

  end subroutine msdparser_finalize


end module saydx_msdparser
