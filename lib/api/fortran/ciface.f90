!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!!> Contains the C-bindings for the saydx library
module saydx_ciface
  use saydx_cinterop, only : c_ptr, c_char, c_bool, c_null_ptr
  implicit none
  private

  public :: c_array_dereference
  public :: c_eventhandler_destroy
  public :: c_eventprinter_create, c_eventprinter_cast_to_eventhandler
  public :: c_msdparser_create, c_msdparser_parse_file, c_msdparser_destroy
  public :: c_node_dereference, c_node_get_name, c_node_has_parent
  public :: c_query_create, c_query_destroy, c_query_get_child, c_query_get_child_data
  public :: c_treebuilder_create, c_treebuilder_cast_to_eventhandler, c_treebuilder_transfer_tree

  interface

    subroutine c_array_dereference(array) bind(C, name='array_dereference')
      import :: c_ptr
      type(c_ptr), value :: array
    end subroutine c_array_dereference


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


    subroutine c_node_dereference(node) bind(C, name='node_dereference')
      import :: c_ptr
      type(c_ptr), value :: node
    end subroutine c_node_dereference


    function c_node_has_parent(node) result(hasparent) bind(C, name='node_has_parent')
      import :: c_ptr, c_bool
      type(c_ptr), value :: node
      logical(c_bool) :: hasparent
    end function c_node_has_parent


    function c_node_get_name(node) result(name) bind(C, name='node_get_name')
      import :: c_ptr
      type(c_ptr), value :: node
      type(c_ptr) :: name
    end function c_node_get_name


    function c_query_create() result(query) bind(C, name='query_create')
      import :: c_ptr
      type(c_ptr) :: query
    end function c_query_create


    subroutine c_query_destroy(query) bind(C, name='query_destroy')
      import :: c_ptr
      type(c_ptr), value :: query
    end subroutine c_query_destroy


    function c_query_get_child(query, node, name, required, child) result(error)&
        & bind(C, name='query_get_child')
      import :: c_ptr, c_char, c_bool
      type(c_ptr), value :: query
      type(c_ptr), value :: node
      character(kind=c_char), intent(in) :: name
      logical(c_bool), value :: required
      type(c_ptr), intent(out) :: child
      type(c_ptr) :: error
    end function c_query_get_child


    function c_query_get_child_data(query, node, name, child, data) result(error)&
        & bind(C, name='query_get_child_data')
      import :: c_ptr, c_char
      type(c_ptr), value :: query
      type(c_ptr), value :: node
      character(kind=c_char), intent(in) :: name
      type(c_ptr), intent(out) :: child
      type(c_ptr), intent(out) :: data
      type(c_ptr) :: error
    end function c_query_get_child_data


    function c_treebuilder_create() result(treebuilder) bind(C, name='treebuilder_create')
      import :: c_ptr
      type(c_ptr) :: treebuilder
    end function c_treebuilder_create


    function c_treebuilder_cast_to_eventhandler(treebuilder) result(eventhandler)&
        & bind(C, name='treebuilder_cast_to_eventhandler')
      import :: c_ptr
      type(c_ptr), value :: treebuilder
      type(c_ptr) :: eventhandler
    end function c_treebuilder_cast_to_eventhandler


    subroutine c_treebuilder_transfer_tree(treebuilder, targetnode)&
        & bind(C, name='treebuilder_transfer_tree')
      import :: c_ptr
      type(c_ptr), value :: treebuilder
      type(c_ptr), intent(out) :: targetnode
    end subroutine c_treebuilder_transfer_tree



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
