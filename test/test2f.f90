program saydx_test2f
  use, intrinsic :: iso_fortran_env, only : stderr => error_unit, stdout => output_unit
  use saydx
  implicit none

  call run_test()

contains

  subroutine run_test()

    type(treebuilder_t), target :: treebuilder
    class(eventhandler_t), pointer :: eventhandler
    type(msdparser_t) :: msdparser
    type(error_t) :: error
    type(node_t) :: root, child
    type(query_t) :: query
    type(array_t) :: data
    integer :: skdver
    integer, pointer :: i4data(:,:)

    call treebuilder_init(treebuilder)
    eventhandler => treebuilder

    call msdparser_init(msdparser, eventhandler)
    call msdparser%parse_file("test2.msd", error=error)
    call check_error(error)

    call treebuilder%transfer_tree(root)

    call query_init(query)

    call query%get_child(root, "slakodef_version", child, required=.false.)
    write(stdout, "(3A,L)") "Child 'slakodef_version' of node '", root%get_name(), "' found: ",&
        & child%is_associated()

    call query%get_child(root, "non_existent_tag", child, required=.false.)
    write(stdout, "(3A,L)") "Child 'non_existent_tag' of node '", root%get_name(), "' found: ",&
        & child%is_associated()

    call query%get_child_data(root, "slakodef_version", data, child=child, error=error)
    call check_error(error)
    write(stdout, "(A)") "Data child 'slakodef_version' found"

    call query%get_child_data(root, "slakodef_version", data, child=child, error=error)
    call check_error(error)
    write(stdout, "(A)") "Data child 'slakodef_version' found (2)"
    !!
    !!call query%get_child_data(root, "slakodef_version", skdver, error)
    !!write(stdout, "(A,I0)") "Integer data of 'slakodef_version': ", skdver
    !!
    !!call query%get_child(root, "two_center_params", child, required=.true.)
    !!write(stdout, "(5A)") "Child '", child%get_name(), "' of node '", root%get_name(), "' found'"
    !!
    !!call query%get_child_data(child, "some_integer_array", i4data)
    !!write(stdout, "(A,I0,1X,I0)") "Rank 2 integer array found. Shape: ", shape(i4data)
    !!write(stdout, "(A)") "Data:"
    !!write(stoud, *) i4data

  end subroutine run_test


  subroutine check_error(error)
    type(error_t), intent(in) :: error

    if (error%is_associated()) then
      write(stderr, "(3A)") "Error occured"
      error stop 1
    end if

  end subroutine check_error


end program saydx_test2f
