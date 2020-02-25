!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!!> Contains the C-bindings for the saydx library
module saydx_ciface
  use saydx_cinterop, only : c_ptr, c_char, c_bool, c_null_ptr
  implicit none
  public

  interface

    subroutine c_eventhandler_destroy(eventhandler) bind(C, name='eventhandler_destroy')
      import :: c_ptr
      type(c_ptr), value :: eventhandler
    end subroutine c_eventhandler_destroy


    function c_eventprinter_create() result(eventprinter) bind(C, name='eventprinter_create')
      import :: c_ptr
      type(c_ptr) :: eventprinter
    end function c_eventprinter_create


    function c_eventprinter_cast_to_eventhandler(eventprinter) result(eventhandler)&
        & bind(C, name='eventprinter_cast_to_eventhandler')
      import :: c_ptr
      type(c_ptr), value :: eventprinter
      type(c_ptr) :: eventhandler
    end function c_eventprinter_cast_to_eventhandler

    function c_msdparser_create(eventhandler) result(msdparser) bind(C, name='msdparser_create')
      import :: c_ptr
      type(c_ptr), value :: eventhandler
      type(c_ptr) :: msdparser
    end function c_msdparser_create


    function c_msdparser_parse_file(msdparser, fname) result(error)&
        & bind(C, name='msdparser_parse_file')
      import :: c_ptr, c_char
      type(c_ptr), value :: msdparser
      character(c_char), intent(in) :: fname
      type(c_ptr) :: error
    end function c_msdparser_parse_file


    subroutine c_msdparser_destroy(msdparser) bind(C, name='msdparser_destroy')
      import :: c_ptr
      type(c_ptr), value :: msdparser
    end subroutine c_msdparser_destroy




    !!subroutine c_error_write(error, file) bind(C, name='error_write')
    !!  import :: c_ptr
    !!  type(c_ptr), value :: error
    !!  type(c_ptr), value :: file
    !!end subroutine c_error_write
    !!
    !!function c_read_msd_file(fname, root) result(error) bind(C, name='read_msd_file')
    !!  import :: c_char, c_ptr
    !!  character(kind=c_char), intent(in) :: fname(*)
    !!  type(c_ptr), intent(out) :: root
    !!  type(c_ptr) :: error
    !!end function c_read_msd_file

    !!subroutine c_query_get_child(query, node, name, required, child) bind(C, name='query_get_child')
    !!  import :: c_ptr, c_char, c_bool
    !!  type(c_ptr), value :: query
    !!  type(c_ptr), value :: node
    !!  character(kind=c_char), intent(in) :: name
    !!  logical(c_bool), value :: required
    !!  type(c_ptr), intent(out) :: child
    !!end subroutine c_query_get_child
    !!
    !!function c_node_get_name(node) result(name) bind(C, name='node_get_name')
    !!  import :: c_ptr
    !!  type(c_ptr), value :: node
    !!  type(c_ptr) :: name
    !!end function c_node_get_name
    !!
    !!subroutine c_query_get_child_data(query, node, name, child, data)&
    !!    & bind(C, name='query_get_child_data')
    !!  import :: c_ptr, c_char
    !!  type(c_ptr), value :: query
    !!  type(c_ptr), value :: node
    !!  character(kind=c_char), intent(in) :: name
    !!  type(c_ptr), intent(out) :: child
    !!  type(c_ptr), intent(out) :: data
    !!end subroutine c_query_get_child_data
    !!
    !!
    !!subroutine c_query_get_child_data_i4(query, node, name, child, data)&
    !!    & bind(C, name='query_get_child_data_i4')
    !!  import :: c_ptr, c_char, c_int
    !!  type(c_ptr), value :: query
    !!  type(c_ptr), value :: node
    !!  character(kind=c_char), intent(in) :: name
    !!  type(c_ptr), intent(out) :: child
    !!  integer(c_int), intent(out) :: data
    !!end subroutine c_query_get_child_data_i4


  end interface

end module saydx_ciface
