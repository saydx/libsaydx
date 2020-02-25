!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

module saydx
  use saydx_commontypes, only : error_t
  use saydx_eventhandler, only : eventhandler_t
  use saydx_eventprinter, only : eventprinter_t, eventprinter_init
  use saydx_msdparser, only: msdparser_t, msdparser_init
  implicit none
  private

  public :: error_t
  public :: msdparser_t, msdparser_init
  public :: eventhandler_t
  public :: eventprinter_t, eventprinter_init


contains


  !!subroutine read_msd_file(fname, root, error)
  !!  character(*), intent(in) :: fname
  !!  type(node_ptr), intent(inout) :: root
  !!  type(error_ptr), intent(out), optional :: error
  !!
  !!  type(error_ptr) :: error_
  !!
  !!  error_%cptr = c_read_msd_file(f_c_string(fname), root%cptr)
  !!  if (error_%is_associated()) then
  !!    if (present(error)) then
  !!      error = error_
  !!      return
  !!    else
  !!      call fatal_error(error_)
  !!    end if
  !!  end if
  !!
  !!end subroutine read_msd_file


end module saydx
