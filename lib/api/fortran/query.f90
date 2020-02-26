!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Wrapper for the query object
module saydx_query
  use saydx_array, only : array_t
  use saydx_cinterop, only : c_null_ptr, c_associated, f_c_string, c_bool
  use saydx_ciface, only : c_query_create, c_query_destroy, c_query_get_child,&
      & c_query_get_child_data
  use saydx_commontypes, only : c_ptr_wrapper_t, error_t, handle_error
  use saydx_node, only : node_t
  implicit none
  private

  public :: query_t, query_init


  type, extends(c_ptr_wrapper_t) :: query_t
  contains
    procedure :: get_child => query_get_child
    procedure, private :: query_get_child_data_array
    generic :: get_child_data => query_get_child_data_array
    !generic :: get_child_data => query_get_child_data_i4
    final :: query_final
  end type query_t


contains

  subroutine query_init(this)
    type(query_t), intent(out) :: this

    this%cptr = c_query_create()

  end subroutine query_init


  subroutine query_final(this)
    type(query_t), intent(inout) :: this

    if (.not. c_associated(this%cptr)) then
      return
    end if
    call c_query_destroy(this%cptr)
    this%cptr = c_null_ptr

  end subroutine query_final


  subroutine query_get_child(this, node, name, child, required, error)
    class(query_t), intent(inout) :: this
    type(node_t), intent(in) :: node
    character(*), intent(in) :: name
    type(node_t), intent(out) :: child
    logical, intent(in), optional :: required
    type(error_t), intent(out), optional :: error

    type(error_t) :: error_

    error_%cptr = c_query_get_child(this%cptr, node%cptr, f_c_string(name),&
        & logical(required, kind=c_bool), child%cptr)
    call handle_error(error_, "Child '" // name // "' of node '" // node%get_name()&
        & // "' not found", error)
    if (present(error)) then
      error = error_
      return
    else if (error_%is_associated()) then
      error stop
    end if

  end subroutine query_get_child


  subroutine query_get_child_data_array(this, node, name, data, child, error)
    class(query_t), intent(inout) :: this
    type(node_t), intent(in) :: node
    character(*), intent(in) :: name
    type(array_t), intent(out) :: data
    type(node_t), intent(out), optional :: child
    type(error_t), intent(out), optional :: error

    type(node_t) :: child_
    type(error_t) :: error_

    error_%cptr = c_query_get_child_data(this%cptr, node%cptr, f_c_string(name), child_%cptr,&
        & data%cptr)
    call handle_error(error_, "Data child '" // name // "' of node '" // node%get_name()&
        & // "' not found", error)
    if (present(child)) then
      call child_%move_to(child)
    end if

  end subroutine query_get_child_data_array


  !!subroutine query_get_child_data_i4(this, node, name, data, child)
  !!  class(query_t), intent(inout) :: this
  !!  type(node_t), intent(in) :: node
  !!  character(*), intent(in) :: name
  !!  integer(c_int), intent(out) :: data
  !!  type(node_t), intent(out), optional :: child
  !!
  !!  type(array_t) :: array
  !!  integer(c_int), pointer :: pdata
  !!
  !!  call c_query_get_child_data_i4(this%cptr, node%cptr, f_c_string(name), child%cptr, data)
  !!
  !!end subroutine query_get_child_data_i4

  subroutine move_to_optval(val, optval)
    type(node_t), intent(inout) :: val
    type(node_t), intent(out), optional :: optval

    if (present(optval)) then
      optval = val
    end if

  end subroutine move_to_optval

end module saydx_query
