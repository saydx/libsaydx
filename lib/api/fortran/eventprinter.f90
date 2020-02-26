!
! Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
! Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
!

!> Wrapper for eventprinter
module saydx_eventprinter
  use saydx_cinterop, only : c_ptr, c_null_ptr
  use saydx_ciface, only : c_eventprinter_create, c_eventprinter_cast_to_eventhandler
  use saydx_eventhandler, only : eventhandler_t, eventhandler_final
  implicit none
  private


  public :: eventprinter_t, eventprinter_init


  type, extends(eventhandler_t) :: eventprinter_t
    private
    type(c_ptr) :: eventprinter_cptr = c_null_ptr
  contains
    final :: eventprinter_final
  end type eventprinter_t


contains

  subroutine eventprinter_init(this)
    type(eventprinter_t), intent(out) :: this

    this%eventprinter_cptr = c_eventprinter_create();
    this%cptr = c_eventprinter_cast_to_eventhandler(this%eventprinter_cptr);

  end subroutine eventprinter_init


  subroutine eventprinter_final(this)
    type(eventprinter_t), intent(inout) :: this

    call eventhandler_final(this%eventhandler_t)

  end subroutine eventprinter_final


end module saydx_eventprinter
