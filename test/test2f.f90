!!>
!!> Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
!!> Use of this source code is governed by a BSD-style
!!> license that can be found in the LICENSE file.
!!>

program saydx_test2
  use, intrinsic :: iso_fortran_env, only : stderr => error_unit, stdout => output_unit
  use saydx
  implicit none


  type(node_ptr) :: root, child
  type(error_ptr) :: error
  type(query_ptr) :: query
  type(array_ptr) :: array
  integer :: skdver


  call read_msd_file("test2.msd", root)
  if (error%is_associated()) then
    write(stderr, "(A)") "Error occured during parsing the msd-file"
    !error_write(error)
    error stop 1
  end if

  call query%get_child(root, "slakodef_version", child, required=.false.)
  print *, "ASSOCIATED: ", child%is_associated()

  if (child%is_associated()) then
    write(stdout, "(5A)") "Child '", child%get_name(), "' of node '", root%get_name(), "' found.'"
  else
    write(stdout, "(5A)") "Child '", child%get_name(), "' of node '", root%get_name(),&
        & " not found.'"
  end if

  call query%get_child(root, "non_existent_tag", required=.false., child=child)
  if (child%is_associated()) then
    write(stdout, "(5A)") "Child '", "non_existent_tag", "' of node '", root%get_name(), " found.'"
  else
    write(stdout, "(5A)") "Child '", "non_existent_tag", "' of node '", root%get_name(),&
        & " not found.'"
  end if

  call query%get_child_data(root, "slakodef_version", array, child=child)
  write(stdout, "(3A)") "Data child '", "slakodef_version", "' found"

  call query%get_child_data(root, "slakodef_version", skdver, child=child)
  write(stdout, "(A,I0)") "Skdef version: ", skdver


end program saydx_test2
