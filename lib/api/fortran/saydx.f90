!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

module saydx
  use saydx_array
  use saydx_commontypes
  use saydx_eventhandler
  use saydx_eventprinter
  use saydx_msdparser
  use saydx_node
  use saydx_query
  use saydx_treebuilder
  implicit none
  private

  public :: array_t
  public :: error_t
  public :: eventhandler_t
  public :: eventprinter_t, eventprinter_init
  public :: msdparser_t, msdparser_init
  public :: node_t
  public :: query_t, query_init
  public :: treebuilder_t, treebuilder_init


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
