!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                                                   !!
!!                   GNU General Public License                      !!
!!                                                                   !!
!! This file is part of the Flexible Modeling System (FMS).          !!
!!                                                                   !!
!! FMS is free software; you can redistribute it and/or modify       !!
!! it and are expected to follow the terms of the GNU General Public !!
!! License as published by the Free Software Foundation.             !!
!!                                                                   !!
!! FMS is distributed in the hope that it will be useful,            !!
!! but WITHOUT ANY WARRANTY; without even the implied warranty of    !!
!! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     !!
!! GNU General Public License for more details.                      !!
!!                                                                   !!
!! You should have received a copy of the GNU General Public License !!
!! along with FMS; if not, write to:                                 !!
!!          Free Software Foundation, Inc.                           !!
!!          59 Temple Place, Suite 330                               !!
!!          Boston, MA  02111-1307  USA                              !!
!! or see:                                                           !!
!!          http://www.gnu.org/licenses/gpl.txt                      !!
!!                                                                   !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef __sgi
#define SYSTEM_CLOCK system_clock_sgi
!#######################################################################
  !mimics F90 SYSTEM_CLOCK intrinsic
subroutine system_clock_sgi( count, count_rate, count_max )
  integer(LONG_KIND), intent(out), optional :: count, count_rate, count_max
  integer(LONG_KIND) :: sgi_tick, sgi_ticks_per_sec, sgi_max_tick
  !sgi_max_tick currently returns 64
  !count must return a number between 0 and count_max
  integer(LONG_KIND), save :: maxtick=0
  if( maxtick.EQ.0 )then
     maxtick = sgi_max_tick() !actually reports #bits in maxtick
     if( maxtick.LT.BIT_SIZE(maxtick) )then
        maxtick = 2**maxtick
     else
        maxtick = huge(maxtick)
     end if
  end if
  if( PRESENT(count) )then
     count = modulo( sgi_tick()-tick0, maxtick )
     !          count = sgi_tick()
  end if
  if( PRESENT(count_rate) )then
     count_rate = sgi_ticks_per_sec()
  end if
  if( PRESENT(count_max) )then
     count_max = maxtick-1
  end if
  return
end subroutine system_clock_sgi

#elif defined(use_libMPI)
#define SYSTEM_CLOCK system_clock_mpi

!#######################################################################
subroutine system_clock_mpi( count, count_rate, count_max )
! There can be one ONE baseline count0 and this routine is
! included in multiple places.
  use mpp_data_mod, only: first_call_system_clock_mpi
  use mpp_data_mod, only: mpi_count0, mpi_tick_rate
!mimics F90 SYSTEM_CLOCK intrinsic
      integer(LONG_KIND), intent(out), optional :: count, count_rate, count_max
!count must return a number between 0 and count_max
      integer(LONG_KIND), parameter :: maxtick=HUGE(count_max)
      if(first_call_system_clock_mpi)then
         first_call_system_clock_mpi=.false.
         mpi_count0 = MPI_WTime()
         mpi_tick_rate = 1.d0/MPI_WTick()
      endif
      if( PRESENT(count) )then
          count = (MPI_WTime()-mpi_count0)*mpi_tick_rate
      end if
      if( PRESENT(count_rate) )then
          count_rate = mpi_tick_rate
      end if
      if( PRESENT(count_max) )then
          count_max = maxtick-1
      end if
      return
    end subroutine system_clock_mpi
#endif
