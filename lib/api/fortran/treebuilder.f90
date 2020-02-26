!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Wrapper for treebuilder
module saydx_treebuilder
  use saydx_cinterop, only : c_ptr, c_null_ptr
  use saydx_ciface, only : c_treebuilder_create, c_treebuilder_cast_to_eventhandler,&
      & c_treebuilder_transfer_tree
  use saydx_eventhandler, only : eventhandler_t, eventhandler_final
  use saydx_node, only : node_t
  implicit none
  private


  public :: treebuilder_t, treebuilder_init


  type, extends(eventhandler_t) :: treebuilder_t
    private
    type(c_ptr) :: treebuilder_cptr = c_null_ptr
  contains
    procedure :: transfer_tree => treebuilder_transfer_tree
    final :: treebuilder_final
  end type treebuilder_t


contains

  subroutine treebuilder_init(this)
    type(treebuilder_t), intent(out) :: this

    this%treebuilder_cptr = c_treebuilder_create();
    this%cptr = c_treebuilder_cast_to_eventhandler(this%treebuilder_cptr);

  end subroutine treebuilder_init


  subroutine treebuilder_final(this)
    type(treebuilder_t), intent(inout) :: this

    call eventhandler_final(this%eventhandler_t)

  end subroutine treebuilder_final


  subroutine treebuilder_transfer_tree(this, node)
    class(treebuilder_t), intent(inout) :: this
    type(node_t), intent(out) :: node

    call c_treebuilder_transfer_tree(this%treebuilder_cptr, node%cptr)

  end subroutine treebuilder_transfer_tree


end module saydx_treebuilder
