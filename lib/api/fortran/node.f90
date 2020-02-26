!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Wrapper for node.
module saydx_node
  use saydx_ciface, only : c_node_dereference, c_node_get_name, c_node_has_parent
  use saydx_cinterop, only : c_ptr, c_null_ptr, c_associated, c_f_string_pointer
  use saydx_common, only : fatal_error
  use saydx_commontypes, only : c_ptr_wrapper_t
  implicit none
  private

  public :: node_t


  type, extends(c_ptr_wrapper_t) :: node_t
  contains
    procedure :: get_name => node_get_name
    procedure :: has_parent => node_has_parent
    final :: node_final
  end type node_t


contains

  function node_get_name(this) result(pstr)
    class(node_t), intent(in) :: this
    character(:), pointer :: pstr

    call c_f_string_pointer(c_node_get_name(this%cptr), pstr)

  end function node_get_name


  function node_has_parent(this) result(hasparent)
    class(node_t), intent(in) :: this
    logical :: hasparent

    hasparent = logical(c_node_has_parent(this%cptr))

  end function node_has_parent


  subroutine node_final(this)
    type(node_t), intent(inout) :: this

    if (.not. c_associated(this%cptr)) then
      return
    end if
    call c_node_dereference(this%cptr)
    this%cptr = c_null_ptr

  end subroutine node_final


end module saydx_node
