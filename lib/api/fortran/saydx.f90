!!>
!!> Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
!!> Use of this source code is governed by a BSD-style
!!> license that can be found in the LICENSE file.
!!>

module saydx
  use, intrinsic :: iso_c_binding, only : c_ptr, c_int, c_char, c_bool, c_null_ptr,&
      & c_associated, c_f_pointer
  use cinterop
  implicit none
  private

  public :: node_ptr, error_ptr, query_ptr, array_ptr
  public :: read_msd_file

  type :: c_ptr_wrapper_t
    type(c_ptr) :: cptr
  contains
    procedure, non_overridable :: is_associated => c_ptr_wrapper_is_associated
  end type c_ptr_wrapper_t


  type, extends(c_ptr_wrapper_t) :: node_ptr
    private
  contains
    procedure :: get_name => node_ptr_get_name
  end type node_ptr


  type, extends(c_ptr_wrapper_t) :: error_ptr
    private
  end type error_ptr


  type, extends(c_ptr_wrapper_t) :: query_ptr
    private
  contains
    private
    procedure, public :: get_child => query_ptr_get_child
    procedure :: query_ptr_get_child_data_array
    generic, public :: get_child_data => query_ptr_get_child_data_array
    procedure :: query_ptr_get_child_data_i4
    generic, public :: get_child_data => query_ptr_get_child_data_i4
  end type query_ptr


  type, extends(c_ptr_wrapper_t) :: array_ptr
    private
  end type array_ptr


  interface
    subroutine c_error_write(error, file) bind(C, name='error_write')
      import :: c_ptr
      type(c_ptr), value :: error
      type(c_ptr), value :: file
    end subroutine c_error_write

    function c_read_msd_file(fname, root) result(error) bind(C, name='read_msd_file')
      import :: c_char, c_ptr
      character(kind=c_char), intent(in) :: fname(*)
      type(c_ptr), intent(out) :: root
      type(c_ptr) :: error
    end function c_read_msd_file

    subroutine c_query_get_child(query, node, name, required, child) bind(C, name='query_get_child')
      import :: c_ptr, c_char, c_bool
      type(c_ptr), value :: query
      type(c_ptr), value :: node
      character(kind=c_char), intent(in) :: name
      logical(c_bool), value :: required
      type(c_ptr), intent(out) :: child
    end subroutine c_query_get_child

    function c_node_get_name(node) result(name) bind(C, name='node_get_name')
      import :: c_ptr
      type(c_ptr), value :: node
      type(c_ptr) :: name
    end function c_node_get_name

    subroutine c_query_get_child_data(query, node, name, child, data)&
        & bind(C, name='query_get_child_data')
      import :: c_ptr, c_char
      type(c_ptr), value :: query
      type(c_ptr), value :: node
      character(kind=c_char), intent(in) :: name
      type(c_ptr), intent(out) :: child
      type(c_ptr), intent(out) :: data
    end subroutine c_query_get_child_data


    subroutine c_query_get_child_data_i4(query, node, name, child, data)&
        & bind(C, name='query_get_child_data_i4')
      import :: c_ptr, c_char, c_int
      type(c_ptr), value :: query
      type(c_ptr), value :: node
      character(kind=c_char), intent(in) :: name
      type(c_ptr), intent(out) :: child
      integer(c_int), intent(out) :: data
    end subroutine c_query_get_child_data_i4


end interface


contains


  function c_ptr_wrapper_is_associated(this) result(isassoc)
    class(c_ptr_wrapper_t), intent(in) :: this
    logical :: isassoc

    isassoc = c_associated(this%cptr)

  end function c_ptr_wrapper_is_associated


  subroutine read_msd_file(fname, root, error)
    character(*), intent(in) :: fname
    type(node_ptr), intent(inout) :: root
    type(error_ptr), intent(out), optional :: error

    type(error_ptr) :: error_

    error_%cptr = c_read_msd_file(f_c_string(fname), root%cptr)
    if (error_%is_associated()) then
      if (present(error)) then
        error = error_
        return
      else
        call fatal_error(error_)
      end if
    end if

  end subroutine read_msd_file



  function node_ptr_get_name(this) result(pstr)
    class(node_ptr), intent(in) :: this
    character(:), pointer :: pstr

    call c_f_string_pointer(c_node_get_name(this%cptr), pstr)

  end function node_ptr_get_name


  subroutine query_ptr_get_child(this, node, name, child, required)
    class(query_ptr), intent(inout) :: this
    type(node_ptr), intent(in) :: node
    character(*), intent(in) :: name
    type(node_ptr), intent(out) :: child
    logical, intent(in), optional :: required

    call c_query_get_child(this%cptr, node%cptr, f_c_string(name), logical(required, kind=c_bool),&
        & child%cptr)

  end subroutine query_ptr_get_child


  subroutine query_ptr_get_child_data_array(this, node, name, data, child)
    class(query_ptr), intent(inout) :: this
    type(node_ptr), intent(in) :: node
    character(*), intent(in) :: name
    type(array_ptr), intent(out) :: data
    type(node_ptr), intent(out), optional :: child

    type(node_ptr) :: child_

    call c_query_get_child_data(this%cptr, node%cptr, f_c_string(name), child_%cptr, data%cptr)
    if (present(child)) then
      child = child_
    end if

  end subroutine query_ptr_get_child_data_array


  subroutine query_ptr_get_child_data_i4(this, node, name, data, child)
    class(query_ptr), intent(inout) :: this
    type(node_ptr), intent(in) :: node
    character(*), intent(in) :: name
    integer(c_int), intent(out) :: data
    type(node_ptr), intent(out), optional :: child

    type(array_ptr) :: array
    integer(c_int), pointer :: pdata

    call c_query_get_child_data_i4(this%cptr, node%cptr, f_c_string(name), child%cptr, data)

  end subroutine query_ptr_get_child_data_i4


  subroutine fatal_error(error)
    type(error_ptr), intent(in) :: error

    call c_error_write(error%cptr, c_null_ptr)
    error stop

  end subroutine fatal_error


end module saydx
