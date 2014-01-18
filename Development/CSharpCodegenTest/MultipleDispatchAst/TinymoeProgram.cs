using System;
using System.Collections.Generic;
using TinymoeDotNet;

namespace TinymoeProgramNamespace
{
	public class TinymoeProgram : TinymoeOperations
	{
		public readonly TinymoeObject standard_library__breaking_repeating = new TinymoeSymbol("standard_library__breaking_repeating");

		public readonly TinymoeObject standard_library__continuing_repeating = new TinymoeSymbol("standard_library__continuing_repeating");

		public readonly TinymoeObject standard_library__raising_exception = new TinymoeSymbol("standard_library__raising_exception");

		public readonly TinymoeObject standard_library__exiting_program = new TinymoeSymbol("standard_library__exiting_program");

		public readonly TinymoeObject standard_library__exiting_block = new TinymoeSymbol("standard_library__exiting_block");

		public class standard_library__continuation_trap : TinymoeObject
		{
			public standard_library__continuation_trap()
			{
				SetField("continuation", null);
				SetField("previous_trap", null);
			}
		}

		public class standard_library__continuation_state : TinymoeObject
		{
			public standard_library__continuation_state()
			{
				SetField("flag", null);
				SetField("argument", null);
				SetField("continuation", null);
				SetField("trap", null);
			}
		}

		public void standard_library__reset_continuation_state__expression_to__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject flag, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			state.SetField("flag", flag);
			state.SetField("argument", null);
			state.SetField("continuation", null);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__call__expression(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__trap__expression_internal(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			state.SetField("trap", the_current_trap);
			Invoke(value, new TinymoeObject[] {
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					_the_result = _result_3;
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__trap__expression(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_internal(
				state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__fall_into_the_previous_trap(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject the_current_trap = null;
			the_current_trap = state.GetField("trap").GetField("continuation");
			TinymoeObject _state_0 = null;
			_state_0 = state;
			Invoke(the_current_trap, new TinymoeObject[] {
				_state_0,
				null
				});
		}

		public void standard_library__break(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__breaking_repeating,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__continue(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__continuing_repeating,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__raise__expression(TinymoeObject state, TinymoeObject exception, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__raising_exception,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("argument", exception);
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__exit_program(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__exiting_program,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__exit_block__expression(TinymoeObject state, TinymoeObject handle, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__exiting_block,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("argument", handle);
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__repeat(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _continuation_1 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation_1, new TinymoeObject[] {
								_state_0,
								_result_3
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_4 = __args___x3[0];
					TinymoeObject _result_5 = __args___x3[1];
					TinymoeObject _select_continuation_8 = null;
					TinymoeObject _select_value_9 = null;
					_select_continuation_8 = new TinymoeFunction(__args___x4 => 
					{
						TinymoeObject _state_6 = __args___x4[0];
						TinymoeObject _result_7 = __args___x4[1];
						Invoke(_continuation, new TinymoeObject[] {
							state,
							_the_result
							});
					});
					_select_value_9 = state.GetField("flag");
					if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__breaking_repeating))).Value)
					{
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							_select_continuation_8
							);
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__continuing_repeating))).Value)
					{
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							new TinymoeFunction(__args___x5 => 
							{
								TinymoeObject _state_12 = __args___x5[0];
								TinymoeObject _result_13 = __args___x5[1];
								standard_library__repeat(
									_state_12,
									body,
									new TinymoeFunction(__args___x6 => 
									{
										TinymoeObject _state_14 = __args___x6[0];
										TinymoeObject _result_15 = __args___x6[1];
										standard_library__call__expression(
											_state_14,
											_result_15,
											_select_continuation_8
											);
									})
									);
							})
							);
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, null))).Value)
					{
						standard_library__repeat(
							_state_4,
							body,
							new TinymoeFunction(__args___x7 => 
							{
								TinymoeObject _state_18 = __args___x7[0];
								TinymoeObject _result_19 = __args___x7[1];
								standard_library__call__expression(
									_state_18,
									_result_19,
									_select_continuation_8
									);
							})
							);
					}
					else
					{
						standard_library__fall_into_the_previous_trap(
							_state_4,
							_select_continuation_8
							);
					}
				})
				);
		}

		public void standard_library__repeat_while__expression(TinymoeObject _state, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__repeat(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(condition, new TinymoeObject[] {
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							standard_library__if__expression(
								_state_4,
								new TinymoeFunction(__args___x3 => standard_library__break(__args___x3[0], __args___x3[1])),
								Not(_result_5),
								new TinymoeFunction(__args___x4 => 
								{
									TinymoeObject _state_6 = __args___x4[0];
									TinymoeObject _result_7 = __args___x4[1];
									Invoke(body, new TinymoeObject[] {
										_state_6,
										_continuation_3
										});
								})
								);
						})
						});
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_0 = __args___x5[0];
					TinymoeObject _result_1 = __args___x5[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__repeat_with__argument_from__expression_to__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject lower_bound, TinymoeObject upper_bound, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_number = null;
			the_current_number = lower_bound;
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			standard_library__repeat_while__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_6 = __args__[0];
					TinymoeObject _continuation_7 = __args__[1];
					Invoke(deal_with__expression, new TinymoeObject[] {
						_state_6,
						the_current_number,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_8 = __args___x2[0];
							TinymoeObject _result_9 = __args___x2[1];
							standard_library__add__expression_to_assignable(
								_state_8,
								new TinymoeInteger(1),
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_10 = __args___x3[0];
									TinymoeObject _continuation_11 = __args___x3[1];
									Invoke(_continuation_11, new TinymoeObject[] {
										_state_10,
										the_current_number
										});
								}),
								new TinymoeFunction(__args___x4 => 
								{
									TinymoeObject _state_12 = __args___x4[0];
									TinymoeObject _input_13 = __args___x4[1];
									TinymoeObject _continuation_14 = __args___x4[2];
									the_current_number = _input_13;
									Invoke(_continuation_14, new TinymoeObject[] {
										_state_12,
										null
										});
								}),
								_continuation_7
								);
						})
						});
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_2 = __args___x5[0];
					TinymoeObject _continuation_3 = __args___x5[1];
					Invoke(_continuation_3, new TinymoeObject[] {
						_state_2,
						LE(the_current_number, upper_bound)
						});
				}),
				new TinymoeFunction(__args___x6 => 
				{
					TinymoeObject _state_4 = __args___x6[0];
					TinymoeObject _result_5 = __args___x6[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			Invoke(items.GetField("$dispatch<>standard_library::repeat_with_$argument_in_$expression"), new TinymoeObject[] {
				_state,
				deal_with__expression,
				items,
				_continuation
				});
		}

		public void standard_library__repeat_with__argument_in__expression_array_(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__repeat_with__argument_from__expression_to__expression(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject index = __args__[1];
					TinymoeObject _continuation_3 = __args__[2];
					Invoke(deal_with__expression, new TinymoeObject[] {
						_state_2,
						ArrayGet(items, index),
						_continuation_3
						});
				}),
				new TinymoeInteger(1),
				ArrayLength(items),
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__if__expression(TinymoeObject _state, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = condition;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(true)))).Value)
			{
				Invoke(body, new TinymoeObject[] {
					_state,
					_select_continuation_2
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__else_if__expression(TinymoeObject _state, TinymoeObject signal, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = signal;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				Invoke(condition, new TinymoeObject[] {
					_state,
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						_the_result = _result_5;
						TinymoeObject _select_continuation_8 = null;
						TinymoeObject _select_value_9 = null;
						_select_continuation_8 = _select_continuation_2;
						_select_value_9 = _the_result;
						if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, new TinymoeBoolean(true)))).Value)
						{
							Invoke(body, new TinymoeObject[] {
								_state_4,
								_select_continuation_8
								});
						}
						else
						{
							Invoke(_select_continuation_8, new TinymoeObject[] {
								_state_4,
								null
								});
						}
					})
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__else(TinymoeObject _state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = signal;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				Invoke(body, new TinymoeObject[] {
					_state,
					_select_continuation_2
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__try(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _continuation_1 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation_1, new TinymoeObject[] {
								_state_0,
								_result_3
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_4 = __args___x3[0];
					TinymoeObject _result_5 = __args___x3[1];
					TinymoeObject _select_continuation_8 = null;
					TinymoeObject _select_value_9 = null;
					_select_continuation_8 = new TinymoeFunction(__args___x4 => 
					{
						TinymoeObject _state_6 = __args___x4[0];
						TinymoeObject _result_7 = __args___x4[1];
						Invoke(_continuation, new TinymoeObject[] {
							state,
							_the_result
							});
					});
					_select_value_9 = state.GetField("flag");
					if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__raising_exception))).Value)
					{
						_the_result = state.GetField("argument");
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							_select_continuation_8
							);
					}
					else
					{
						standard_library__fall_into_the_previous_trap(
							_state_4,
							_select_continuation_8
							);
					}
				})
				);
		}

		public void standard_library__else_try(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					standard_library__trap__expression(
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _continuation_5 = __args___x2[1];
							Invoke(body, new TinymoeObject[] {
								_state_4,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_6 = __args___x3[0];
									TinymoeObject _result_7 = __args___x3[1];
									Invoke(_continuation_5, new TinymoeObject[] {
										_state_4,
										_result_7
										});
								})
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_8 = __args___x4[0];
							TinymoeObject _result_9 = __args___x4[1];
							TinymoeObject _select_continuation_12 = null;
							TinymoeObject _select_value_13 = null;
							_select_continuation_12 = _continuation_3;
							_select_value_13 = state.GetField("flag");
							if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_13, standard_library__raising_exception))).Value)
							{
								_the_result = state.GetField("argument");
								standard_library__reset_continuation_state__expression_to__expression(
									_state_8,
									state,
									null,
									_select_continuation_12
									);
							}
							else
							{
								standard_library__fall_into_the_previous_trap(
									_state_8,
									_select_continuation_12
									);
							}
						})
						);
				}),
				NE(signal, null),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_0 = __args___x5[0];
					TinymoeObject _result_1 = __args___x5[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__catch__argument(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				state,
				body,
				NE(signal, null),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__finally(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(body, new TinymoeObject[] {
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__named_block__argument(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject handle = null;
			handle = new TinymoeObject().SetFields(new TinymoeObject[] {}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__trap__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_2,
						handle,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							Invoke(_continuation_3, new TinymoeObject[] {
								_state_2,
								_result_5
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_6 = __args___x3[0];
					TinymoeObject _result_7 = __args___x3[1];
					standard_library__if__expression(
						_state_6,
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_10 = __args___x4[0];
							TinymoeObject _continuation_11 = __args___x4[1];
							standard_library__reset_continuation_state__expression_to__expression(
								_state_10,
								state,
								null,
								_continuation_11
								);
						}),
						And(EQ(state.GetField("flag"), standard_library__exiting_block), EQ(state.GetField("argument"), handle)),
						new TinymoeFunction(__args___x5 => 
						{
							TinymoeObject _state_8 = __args___x5[0];
							TinymoeObject _result_9 = __args___x5[1];
							standard_library__else_if__expression(
								_state_8,
								_result_9,
								new TinymoeFunction(__args___x6 => standard_library__fall_into_the_previous_trap(__args___x6[0], __args___x6[1])),
								new TinymoeFunction(__args___x7 => 
								{
									TinymoeObject _state_14 = __args___x7[0];
									TinymoeObject _continuation_15 = __args___x7[1];
									Invoke(_continuation_15, new TinymoeObject[] {
										_state_14,
										NE(state.GetField("flag"), null)
										});
								}),
								new TinymoeFunction(__args___x8 => 
								{
									TinymoeObject _state_16 = __args___x8[0];
									TinymoeObject _result_17 = __args___x8[1];
									Invoke(_continuation, new TinymoeObject[] {
										state,
										_the_result
										});
								})
								);
						})
						);
				})
				);
		}

		public void standard_library__add__expression_to_assignable(TinymoeObject _state, TinymoeObject value, TinymoeObject _read_variable, TinymoeObject _write_variable, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_read_variable, new TinymoeObject[] {
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_write_variable, new TinymoeObject[] {
						_state_0,
						Add(_result_1, value),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				});
		}

		public void standard_library__substract__expression_from_assignable(TinymoeObject _state, TinymoeObject value, TinymoeObject _read_variable, TinymoeObject _write_variable, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_read_variable, new TinymoeObject[] {
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_write_variable, new TinymoeObject[] {
						_state_0,
						Sub(_result_1, value),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				});
		}

		public void standard_library__boolean_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToBoolean")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__integer_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToInteger")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__floating_point_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToFloat")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__string_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToString")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void geometry__square_root_of__primitive(TinymoeObject _state, TinymoeObject number, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("Sqrt")), new TinymoeObject[] {
				_state,
				number,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void geometry__print__expression(TinymoeObject _state, TinymoeObject message, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("Print")), new TinymoeObject[] {
				_state,
				message,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public class geometry__rectangle : TinymoeObject
		{
			public geometry__rectangle()
			{
				SetField("width", null);
				SetField("height", null);
			}
		}

		public class geometry__triangle : TinymoeObject
		{
			public geometry__triangle()
			{
				SetField("a", null);
				SetField("b", null);
				SetField("c", null);
			}
		}

		public class geometry__circle : TinymoeObject
		{
			public geometry__circle()
			{
				SetField("radius", null);
			}
		}

		public void geometry__area_of__primitive(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			Invoke(shape.GetField("$dispatch<>geometry::area_of_$primitive"), new TinymoeObject[] {
				_state,
				shape,
				_continuation
				});
		}

		public void geometry__area_of__primitive_rectangle_(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = Mul(shape.GetField("width"), shape.GetField("height"));
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void geometry__area_of__primitive_triangle_(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject a = null;
			a = shape.GetField("a");
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			TinymoeObject b = null;
			b = shape.GetField("b");
			TinymoeObject _state_2 = null;
			_state_2 = _state_0;
			TinymoeObject c = null;
			c = shape.GetField("c");
			TinymoeObject _state_4 = null;
			_state_4 = _state_2;
			TinymoeObject p = null;
			p = Div(Add(Add(a, b), c), new TinymoeInteger(2));
			TinymoeObject _state_6 = null;
			_state_6 = _state_4;
			geometry__square_root_of__primitive(
				_state_6,
				Mul(Mul(Mul(p, Sub(p, a)), Sub(p, b)), Sub(p, c)),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_8 = __args__[0];
					TinymoeObject _result_9 = __args__[1];
					_the_result = _result_9;
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void geometry__area_of__primitive_circle_(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject r = null;
			r = shape.GetField("radius");
			_the_result = Mul(Mul(r, r), new TinymoeFloat(3.14));
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void geometry___primitive_and__expression_are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>geometry::$primitive_and_$expression_are_the_same_shape"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void geometry___primitive_rectangle__and__expression_rectangle__are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new TinymoeBoolean(true);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void geometry___primitive_triangle__and__expression_triangle__are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new TinymoeBoolean(true);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void geometry___primitive_circle__and__expression_circle__are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new TinymoeBoolean(true);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void geometry__main(TinymoeObject _state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject shape_one = null;
			shape_one = new geometry__triangle().SetFields(new TinymoeObject[] {new TinymoeInteger(2), new TinymoeInteger(3), new TinymoeInteger(4)}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			TinymoeObject shape_two = null;
			shape_two = new geometry__rectangle().SetFields(new TinymoeObject[] {new TinymoeInteger(1), new TinymoeInteger(2)}).FinishConstruction();
			TinymoeObject _state_2 = null;
			_state_2 = _state_0;
			geometry___primitive_and__expression_are_the_same_shape(
				_state_2,
				shape_one,
				shape_two,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_4 = __args__[0];
					TinymoeObject _result_5 = __args__[1];
					standard_library__if__expression(
						_state_4,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_8 = __args___x2[0];
							TinymoeObject _continuation_9 = __args___x2[1];
							geometry__print__expression(
								_state_8,
								new TinymoeString("This world is mad!"),
								_continuation_9
								);
						}),
						_result_5,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_6 = __args___x3[0];
							TinymoeObject _result_7 = __args___x3[1];
							standard_library__else(
								_state_6,
								_result_7,
								new TinymoeFunction(__args___x4 => 
								{
									TinymoeObject _state_14 = __args___x4[0];
									TinymoeObject _continuation_15 = __args___x4[1];
									geometry__print__expression(
										_state_14,
										new TinymoeString("Triangle and rectangle are not the same shape!"),
										_continuation_15
										);
								}),
								new TinymoeFunction(__args___x5 => 
								{
									TinymoeObject _state_12 = __args___x5[0];
									TinymoeObject _result_13 = __args___x5[1];
									Invoke(_continuation, new TinymoeObject[] {
										_state,
										_the_result
										});
								})
								);
						})
						);
				})
				);
		}

		public void _dispatch_fail__standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				new TinymoeString("Only enumerable object (like containers) can be iterated using the repeat-with-in statement."),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void TinymoeArray___dispatch__standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			standard_library__repeat_with__argument_in__expression_array_(
				_state,
				deal_with__expression,
				items,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__repeat_with__argument_in__expression_x2(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__repeat_with__argument_in__expression(
				_state,
				deal_with__expression,
				items,
				_continuation
				);
		}

		public void _dispatch_fail__geometry__area_of__primitive(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				new TinymoeString("This is not a shape."),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void geometry__rectangle___dispatch__geometry__area_of__primitive(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			geometry__area_of__primitive_rectangle_(
				_state,
				shape,
				_continuation
				);
		}

		public void geometry__triangle___dispatch__geometry__area_of__primitive_x2(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			geometry__area_of__primitive_triangle_(
				_state,
				shape,
				_continuation
				);
		}

		public void geometry__circle___dispatch__geometry__area_of__primitive_x3(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			geometry__area_of__primitive_circle_(
				_state,
				shape,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__geometry__area_of__primitive_x4(TinymoeObject _state, TinymoeObject shape, TinymoeObject _continuation)
		{
			_dispatch_fail__geometry__area_of__primitive(
				_state,
				shape,
				_continuation
				);
		}

		public void _dispatch_fail__geometry___primitive_and__expression_are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new TinymoeBoolean(false);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void geometry__rectangle___dispatch__geometry___primitive_and__expression_are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<geometry::rectangle>geometry::$primitive_and_$expression_are_the_same_shape"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void geometry__rectangle___dispatch_geometry__rectangle_geometry___primitive_and__expression_are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			geometry___primitive_rectangle__and__expression_rectangle__are_the_same_shape(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void geometry__triangle___dispatch__geometry___primitive_and__expression_are_the_same_shape_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<geometry::triangle>geometry::$primitive_and_$expression_are_the_same_shape"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void geometry__triangle___dispatch_geometry__triangle_geometry___primitive_and__expression_are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			geometry___primitive_triangle__and__expression_triangle__are_the_same_shape(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void geometry__circle___dispatch__geometry___primitive_and__expression_are_the_same_shape_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<geometry::circle>geometry::$primitive_and_$expression_are_the_same_shape"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void geometry__circle___dispatch_geometry__circle_geometry___primitive_and__expression_are_the_same_shape(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			geometry___primitive_circle__and__expression_circle__are_the_same_shape(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__geometry___primitive_and__expression_are_the_same_shape_x4(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__geometry___primitive_and__expression_are_the_same_shape(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch_geometry__circle_geometry___primitive_and__expression_are_the_same_shape_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__geometry___primitive_and__expression_are_the_same_shape(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch_geometry__rectangle_geometry___primitive_and__expression_are_the_same_shape_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__geometry___primitive_and__expression_are_the_same_shape(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch_geometry__triangle_geometry___primitive_and__expression_are_the_same_shape_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__geometry___primitive_and__expression_are_the_same_shape(
				_state,
				a,
				b,
				_continuation
				);
		}

		public TinymoeProgram()
		{
			TinymoeObject.SetExtension(
				typeof(TinymoeArray),
				"$dispatch<>standard_library::repeat_with_$argument_in_$expression",
				new TinymoeFunction(__args__ => TinymoeArray___dispatch__standard_library__repeat_with__argument_in__expression(__args__[0], __args__[1], __args__[2], __args__[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::repeat_with_$argument_in_$expression",
				new TinymoeFunction(__args___x2 => TinymoeObject___dispatch__standard_library__repeat_with__argument_in__expression_x2(__args___x2[0], __args___x2[1], __args___x2[2], __args___x2[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__rectangle),
				"$dispatch<>geometry::area_of_$primitive",
				new TinymoeFunction(__args___x3 => geometry__rectangle___dispatch__geometry__area_of__primitive(__args___x3[0], __args___x3[1], __args___x3[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__triangle),
				"$dispatch<>geometry::area_of_$primitive",
				new TinymoeFunction(__args___x4 => geometry__triangle___dispatch__geometry__area_of__primitive_x2(__args___x4[0], __args___x4[1], __args___x4[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__circle),
				"$dispatch<>geometry::area_of_$primitive",
				new TinymoeFunction(__args___x5 => geometry__circle___dispatch__geometry__area_of__primitive_x3(__args___x5[0], __args___x5[1], __args___x5[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>geometry::area_of_$primitive",
				new TinymoeFunction(__args___x6 => TinymoeObject___dispatch__geometry__area_of__primitive_x4(__args___x6[0], __args___x6[1], __args___x6[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__rectangle),
				"$dispatch<>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x7 => geometry__rectangle___dispatch__geometry___primitive_and__expression_are_the_same_shape(__args___x7[0], __args___x7[1], __args___x7[2], __args___x7[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__rectangle),
				"$dispatch<geometry::rectangle>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x8 => geometry__rectangle___dispatch_geometry__rectangle_geometry___primitive_and__expression_are_the_same_shape(__args___x8[0], __args___x8[1], __args___x8[2], __args___x8[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__triangle),
				"$dispatch<>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x9 => geometry__triangle___dispatch__geometry___primitive_and__expression_are_the_same_shape_x2(__args___x9[0], __args___x9[1], __args___x9[2], __args___x9[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__triangle),
				"$dispatch<geometry::triangle>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x10 => geometry__triangle___dispatch_geometry__triangle_geometry___primitive_and__expression_are_the_same_shape(__args___x10[0], __args___x10[1], __args___x10[2], __args___x10[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__circle),
				"$dispatch<>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x11 => geometry__circle___dispatch__geometry___primitive_and__expression_are_the_same_shape_x3(__args___x11[0], __args___x11[1], __args___x11[2], __args___x11[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(geometry__circle),
				"$dispatch<geometry::circle>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x12 => geometry__circle___dispatch_geometry__circle_geometry___primitive_and__expression_are_the_same_shape(__args___x12[0], __args___x12[1], __args___x12[2], __args___x12[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x13 => TinymoeObject___dispatch__geometry___primitive_and__expression_are_the_same_shape_x4(__args___x13[0], __args___x13[1], __args___x13[2], __args___x13[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<geometry::circle>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x14 => TinymoeObject___dispatch_geometry__circle_geometry___primitive_and__expression_are_the_same_shape_x2(__args___x14[0], __args___x14[1], __args___x14[2], __args___x14[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<geometry::rectangle>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x15 => TinymoeObject___dispatch_geometry__rectangle_geometry___primitive_and__expression_are_the_same_shape_x2(__args___x15[0], __args___x15[1], __args___x15[2], __args___x15[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<geometry::triangle>geometry::$primitive_and_$expression_are_the_same_shape",
				new TinymoeFunction(__args___x16 => TinymoeObject___dispatch_geometry__triangle_geometry___primitive_and__expression_are_the_same_shape_x2(__args___x16[0], __args___x16[1], __args___x16[2], __args___x16[3]))
				); 
		}

		static void Main(string[] args)
		{
			var program = new TinymoeProgram();
			var continuation = new TinymoeFunction((TinymoeObject[] arguments) =>
			{
			});
			var trap = new TinymoeProgram.standard_library__continuation_trap();
			trap.SetField("continuation", continuation);
			var state = new TinymoeProgram.standard_library__continuation_state();
			state.SetField("trap", trap);
			program.geometry__main(state, continuation);
		}
	}
}
