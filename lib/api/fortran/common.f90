!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Some commonly used stuff.
module saydx_common
  use, intrinsic :: iso_fortran_env, only : stdin => input_unit, stdout => output_unit,&
      & stderr => error_unit
  implicit none
  private

  public :: stdin, stdout, stderr
  public :: fatal_error

contains


  subroutine fatal_error(msg)
    character(*), intent(in) :: msg

    write(stderr, "(2A)") "FATAL ERROR: ", msg
    error stop

  end subroutine fatal_error


end module saydx_common
