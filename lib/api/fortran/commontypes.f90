module saydx_commontypes
  use saydx_cinterop, only : c_ptr, c_null_ptr, c_associated
  implicit none
  private

  public :: c_ptr_wrapper_t, error_t


  type :: c_ptr_wrapper_t
    type(c_ptr) :: cptr = c_null_ptr
  contains
    procedure, non_overridable :: is_associated => c_ptr_wrapper_is_associated
  end type c_ptr_wrapper_t


  !!type, extends(c_ptr_wrapper_t) :: node_ptr
  !!  private
  !!contains
  !!  procedure :: get_name => node_ptr_get_name
  !!end type node_ptr


  type, extends(c_ptr_wrapper_t) :: error_t
    private
  end type error_t


contains

  function c_ptr_wrapper_is_associated(this) result(isassoc)
    class(c_ptr_wrapper_t), intent(in) :: this
    logical :: isassoc

    isassoc = c_associated(this%cptr)

  end function c_ptr_wrapper_is_associated

  !!
  !!function node_ptr_get_name(this) result(pstr)
  !!  class(node_ptr), intent(in) :: this
  !!  character(:), pointer :: pstr
  !!
  !!  call c_f_string_pointer(c_node_get_name(this%cptr), pstr)
  !!
  !!end function node_ptr_get_name
  !!
  !!subroutine query_ptr_get_child(this, node, name, child, required)
  !!  class(query_ptr), intent(inout) :: this
  !!  type(node_ptr), intent(in) :: node
  !!  character(*), intent(in) :: name
  !!  type(node_ptr), intent(out) :: child
  !!  logical, intent(in), optional :: required
  !!
  !!  call c_query_get_child(this%cptr, node%cptr, f_c_string(name), logical(required, kind=c_bool),&
  !!      & child%cptr)
  !!
  !!end subroutine query_ptr_get_child
  !!
  !!
  !!subroutine query_ptr_get_child_data_array(this, node, name, data, child)
  !!  class(query_ptr), intent(inout) :: this
  !!  type(node_ptr), intent(in) :: node
  !!  character(*), intent(in) :: name
  !!  type(array_ptr), intent(out) :: data
  !!  type(node_ptr), intent(out), optional :: child
  !!
  !!  type(node_ptr) :: child_
  !!
  !!  call c_query_get_child_data(this%cptr, node%cptr, f_c_string(name), child_%cptr, data%cptr)
  !!  if (present(child)) then
  !!    child = child_
  !!  end if
  !!
  !!end subroutine query_ptr_get_child_data_array
  !!
  !!
  !!subroutine query_ptr_get_child_data_i4(this, node, name, data, child)
  !!  class(query_ptr), intent(inout) :: this
  !!  type(node_ptr), intent(in) :: node
  !!  character(*), intent(in) :: name
  !!  integer(c_int), intent(out) :: data
  !!  type(node_ptr), intent(out), optional :: child
  !!
  !!  type(array_ptr) :: array
  !!  integer(c_int), pointer :: pdata
  !!
  !!  call c_query_get_child_data_i4(this%cptr, node%cptr, f_c_string(name), child%cptr, data)
  !!
  !!end subroutine query_ptr_get_child_data_i4



end module saydx_commontypes
