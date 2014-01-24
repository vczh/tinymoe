using System;
using System.Collections.Generic;
using TinymoeDotNet;

namespace TinymoeProgramNamespace
{
	public class TinymoeProgram : TinymoeOperations
	{
		public void standard_library__set_fall_back_counter_of__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject top_trap = null;
			top_trap = state.GetField("argument").GetField("trap");
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			TinymoeObject current_trap = null;
			current_trap = state.GetField("trap");
			TinymoeObject _state_2 = null;
			_state_2 = _state_0;
			standard_library__fall_back_counter_between__expression_and__expression_from__primitive(
				_state_2,
				top_trap,
				current_trap,
				Negative(new TinymoeInteger(1)),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_4 = __args__[0];
					TinymoeObject _result_5 = __args__[1];
					TinymoeObject counter = null;
					counter = _result_5;
					state.GetField("argument").SetField("fall_back_counter", counter);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(TinymoeObject _state, TinymoeObject trap, TinymoeObject fall_back_trap, TinymoeObject counter, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
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
			_select_value_3 = counter;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeInteger(0)))).Value)
			{
				_the_result = trap;
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
			else
			{
				standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(
					_state,
					trap,
					fall_back_trap.GetField("previous_trap"),
					Sub(counter, new TinymoeInteger(1)),
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						TinymoeObject previous_trap = null;
						previous_trap = _result_5;
						_the_result = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {fall_back_trap.GetField("continuation"), previous_trap}).FinishConstruction();
						Invoke(_select_continuation_2, new TinymoeObject[] {
							_state_4,
							_result_5
							});
					})
					);
			}
		}

		public void standard_library__trap__expression_with_fall_back_enabled_internal(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
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

		public void standard_library__trap__expression_with_fall_back_enabled(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_with_fall_back_enabled_internal(
				state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					standard_library__set_fall_back_counter_of__expression(
						_state_0,
						state,
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

		public void standard_library__trap__expression_with_fall_back__expression_restored_internal(TinymoeObject state, TinymoeObject value, TinymoeObject fall_back, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(
				_state_0,
				the_current_trap,
				fall_back.GetField("trap"),
				fall_back.GetField("fall_back_counter"),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					TinymoeObject new_trap = null;
					new_trap = _result_3;
					state.SetField("trap", new_trap);
					Invoke(value, new TinymoeObject[] {
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							_the_result = _result_5;
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__trap__expression_with_fall_back__expression_restored(TinymoeObject state, TinymoeObject value, TinymoeObject fall_back, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_with_fall_back__expression_restored_internal(
				state,
				value,
				fall_back,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					standard_library__set_fall_back_counter_of__expression(
						_state_0,
						state,
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

		public void standard_library__create_closure_to_assignable(TinymoeObject _state, TinymoeObject body, TinymoeObject _read_closure, TinymoeObject _write_closure, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_write_closure, new TinymoeObject[] {
				_state,
				body,
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

		public void standard_library__new_coroutine_from__primitive(TinymoeObject _state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new standard_library__continuation_coroutine().SetFields(new TinymoeObject[] {}).FinishConstruction();
			_the_result.SetField("body", body);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__coroutine__expression_stopped(TinymoeObject _state, TinymoeObject coroutine, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = And(EQ(coroutine.GetField("body"), null), EQ(coroutine.GetField("continuation"), null));
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__run_coroutine__expression(TinymoeObject state, TinymoeObject coroutine, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject need_to_exit = null;
			need_to_exit = new TinymoeBoolean(false);
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__if__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_4 = __args__[0];
					TinymoeObject _continuation_5 = __args__[1];
					standard_library__trap__expression_with_fall_back_enabled(
						_state_4,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_6 = __args___x2[0];
							TinymoeObject _continuation_7 = __args___x2[1];
							Invoke(coroutine.GetField("body"), new TinymoeObject[] {
								_state_6,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_8 = __args___x3[0];
									TinymoeObject _result_9 = __args___x3[1];
									Invoke(_continuation_7, new TinymoeObject[] {
										_state_6,
										_result_9
										});
								})
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_10 = __args___x4[0];
							TinymoeObject _result_11 = __args___x4[1];
							coroutine.SetField("body", null);
							Invoke(_continuation_5, new TinymoeObject[] {
								_state_10,
								_result_11
								});
						})
						);
				}),
				NE(coroutine.GetField("body"), null),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_2 = __args___x5[0];
					TinymoeObject _result_3 = __args___x5[1];
					standard_library__else_if__expression(
						_state_2,
						_result_3,
						new TinymoeFunction(__args___x6 => 
						{
							TinymoeObject _state_16 = __args___x6[0];
							TinymoeObject _continuation_17 = __args___x6[1];
							standard_library__trap__expression_with_fall_back__expression_restored(
								_state_16,
								new TinymoeFunction(__args___x7 => 
								{
									TinymoeObject _state_18 = __args___x7[0];
									TinymoeObject _continuation_19 = __args___x7[1];
									Invoke(coroutine.GetField("continuation"), new TinymoeObject[] {
										_state_18,
										null
										});
								}),
								coroutine,
								_continuation_17
								);
						}),
						new TinymoeFunction(__args___x8 => 
						{
							TinymoeObject _state_12 = __args___x8[0];
							TinymoeObject _continuation_13 = __args___x8[1];
							Invoke(_continuation_13, new TinymoeObject[] {
								_state_12,
								NE(coroutine.GetField("continuation"), null)
								});
						}),
						new TinymoeFunction(__args___x9 => 
						{
							TinymoeObject _state_14 = __args___x9[0];
							TinymoeObject _result_15 = __args___x9[1];
							standard_library__else(
								_state_14,
								_result_15,
								new TinymoeFunction(__args___x10 => 
								{
									TinymoeObject _state_26 = __args___x10[0];
									TinymoeObject _continuation_27 = __args___x10[1];
									need_to_exit = new TinymoeBoolean(true);
									Invoke(_continuation_27, new TinymoeObject[] {
										_state_26,
										null
										});
								}),
								new TinymoeFunction(__args___x11 => 
								{
									TinymoeObject _state_24 = __args___x11[0];
									TinymoeObject _result_25 = __args___x11[1];
									standard_library__if__expression(
										_state_24,
										new TinymoeFunction(__args___x12 => 
										{
											TinymoeObject _state_30 = __args___x12[0];
											TinymoeObject _continuation_31 = __args___x12[1];
											coroutine.SetField("trap", state.GetField("argument").GetField("trap"));
											coroutine.SetField("fall_back_counter", state.GetField("argument").GetField("fall_back_counter"));
											Invoke(_continuation_31, new TinymoeObject[] {
												_state_30,
												null
												});
										}),
										And(Not(need_to_exit), new TinymoeBoolean(state.GetField("argument") is standard_library__continuation_fall_back_argument)),
										new TinymoeFunction(__args___x13 => 
										{
											TinymoeObject _state_28 = __args___x13[0];
											TinymoeObject _result_29 = __args___x13[1];
											Invoke(continuation, new TinymoeObject[] {
												state,
												_the_result
												});
										})
										);
								})
								);
						})
						);
				})
				);
		}

		public void standard_library__continue_coroutine__expression(TinymoeObject state, TinymoeObject coroutine, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			coroutine.SetField("continuation", state.GetField("continuation"));
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				null,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__stop_coroutine__expression(TinymoeObject state, TinymoeObject coroutine, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			coroutine.SetField("continuation", null);
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				null,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__pause_coroutine_to__expression_with__expression_and__expression(TinymoeObject state, TinymoeObject flag, TinymoeObject coroutine_continuation, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				flag,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__enable_fall_back_to__expression_with__expression_and__expression(
						_state_0,
						state,
						coroutine_continuation,
						value,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							standard_library__fall_into_the_previous_trap(
								_state_2,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_4 = __args___x3[0];
									TinymoeObject _result_5 = __args___x3[1];
									Invoke(continuation, new TinymoeObject[] {
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
			the_current_number = Sub(lower_bound, new TinymoeInteger(1));
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			standard_library__repeat_while__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_6 = __args__[0];
					TinymoeObject _continuation_7 = __args__[1];
					standard_library__add__expression_to_assignable(
						_state_6,
						new TinymoeInteger(1),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_8 = __args___x2[0];
							TinymoeObject _continuation_9 = __args___x2[1];
							Invoke(_continuation_9, new TinymoeObject[] {
								_state_8,
								the_current_number
								});
						}),
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_10 = __args___x3[0];
							TinymoeObject _input_11 = __args___x3[1];
							TinymoeObject _continuation_12 = __args___x3[2];
							the_current_number = _input_11;
							Invoke(_continuation_12, new TinymoeObject[] {
								_state_10,
								null
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_13 = __args___x4[0];
							TinymoeObject _result_14 = __args___x4[1];
							Invoke(deal_with__expression, new TinymoeObject[] {
								_state_13,
								the_current_number,
								_continuation_7
								});
						})
						);
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_2 = __args___x5[0];
					TinymoeObject _continuation_3 = __args___x5[1];
					Invoke(_continuation_3, new TinymoeObject[] {
						_state_2,
						LT(the_current_number, upper_bound)
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
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, null))).Value)
					{
						Invoke(_select_continuation_8, new TinymoeObject[] {
							_state_4,
							null
							});
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
							else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_13, null))).Value)
							{
								Invoke(_select_continuation_12, new TinymoeObject[] {
									_state_8,
									null
									});
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

		public void standard_library__catch__argument(TinymoeObject state, TinymoeObject signal, TinymoeObject deal_with__expression, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(deal_with__expression, new TinymoeObject[] {
						_state_2,
						signal,
						_continuation_3
						});
				}),
				NE(signal, null),
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
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

		public void standard_library__number_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			Invoke(value.GetField("$dispatch<>standard_library::number_of_$primitive"), new TinymoeObject[] {
				_state,
				value,
				_continuation
				});
		}

		public void standard_library__number_of__primitive_integer_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = value;
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__number_of__primitive_float_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = value;
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__number_of__primitive_string_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("s_to_n")), new TinymoeObject[] {
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
			Invoke(value.GetField("$dispatch<>standard_library::integer_of_$primitive"), new TinymoeObject[] {
				_state,
				value,
				_continuation
				});
		}

		public void standard_library__integer_of__primitive_integer_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = value;
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__integer_of__primitive_float_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f_to_i")), new TinymoeObject[] {
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

		public void standard_library__integer_of__primitive_string_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("s_to_i")), new TinymoeObject[] {
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

		public void standard_library__float_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			Invoke(value.GetField("$dispatch<>standard_library::float_of_$primitive"), new TinymoeObject[] {
				_state,
				value,
				_continuation
				});
		}

		public void standard_library__float_of__primitive_integer_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i_to_f")), new TinymoeObject[] {
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

		public void standard_library__float_of__primitive_float_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = value;
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__float_of__primitive_string_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("s_to_f")), new TinymoeObject[] {
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
			Invoke(GetExternalFunction(new TinymoeString("to_s")), new TinymoeObject[] {
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

		public void standard_library__operator_POS__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			Invoke(value.GetField("$dispatch<>standard_library::operator_POS_$primitive"), new TinymoeObject[] {
				_state,
				value,
				_continuation
				});
		}

		public void standard_library__operator_POS__primitive_integer_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("+i")), new TinymoeObject[] {
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

		public void standard_library__operator_POS__primitive_float_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("+f")), new TinymoeObject[] {
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

		public void standard_library__operator_NEG__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			Invoke(value.GetField("$dispatch<>standard_library::operator_NEG_$primitive"), new TinymoeObject[] {
				_state,
				value,
				_continuation
				});
		}

		public void standard_library__operator_NEG__primitive_integer_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("-i")), new TinymoeObject[] {
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

		public void standard_library__operator_NEG__primitive_float_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("-f")), new TinymoeObject[] {
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

		public void standard_library__operator_NOT__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			Invoke(value.GetField("$dispatch<>standard_library::operator_NOT_$primitive"), new TinymoeObject[] {
				_state,
				value,
				_continuation
				});
		}

		public void standard_library__operator_NOT__primitive_boolean_(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("!b")), new TinymoeObject[] {
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

		public void standard_library__operator__expression_CONCAT__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_CONCAT_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_string__CONCAT__primitive_string_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("s&s")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_ADD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_ADD_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__ADD__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i+i")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__ADD__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f+f")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__ADD__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Add(_result_1, b);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_float__ADD__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Add(_var_2, _result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_SUB__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_SUB_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__SUB__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i-i")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__SUB__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f-f")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__SUB__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Sub(_result_1, b);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_float__SUB__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Sub(_var_2, _result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_MUL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_MUL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__MUL__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i*i")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__MUL__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f*f")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__MUL__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Mul(_result_1, b);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_float__MUL__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Mul(_var_2, _result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_DIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_DIV_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__DIV__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i/i")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__DIV__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f/f")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__DIV__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Div(_result_1, b);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_float__DIV__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Div(_var_2, _result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_INTDIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_INTDIV_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__INTDIV__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("ii")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__INTDIV__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("ff")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__INTDIV__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = IntDiv(_result_1, b);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_float__INTDIV__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = IntDiv(_var_2, _result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_MOD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_MOD_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__MOD__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i%i")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__MOD__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f%f")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__MOD__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Mod(_result_1, b);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_float__MOD__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Mod(_var_2, _result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_EQUAL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__EQUAL__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i_e_i")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__EQUAL__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f_e_f")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_string__EQUAL__primitive_string_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("s_e_s")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_boolean__EQUAL__primitive_boolean_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("b_e_b")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__EQUAL__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_EQUAL__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_result_1,
						b,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_3 = __args___x3[0];
							TinymoeObject _result_4 = __args___x3[1];
							_the_result = _result_4;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_float__EQUAL__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_EQUAL__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			TinymoeObject _var_3 = null;
			_var_3 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_var_3,
						_result_1,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_4 = __args___x3[0];
							TinymoeObject _result_5 = __args___x3[1];
							_the_result = _result_5;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_string__EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_EQUAL__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			TinymoeObject _var_3 = null;
			_var_3 = a;
			standard_library__string_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_var_3,
						_result_1,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_4 = __args___x3[0];
							TinymoeObject _result_5 = __args___x3[1];
							_the_result = _result_5;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_EQUAL__primitive_string_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_EQUAL__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			standard_library__string_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_result_1,
						b,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_3 = __args___x3[0];
							TinymoeObject _result_4 = __args___x3[1];
							_the_result = _result_4;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_COMPARE_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_integer__COMPARE__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("i_c_i")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_float__COMPARE__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("f_c_f")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_string__COMPARE__primitive_string_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("s_c_s")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_integer__COMPARE__primitive_float_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_COMPARE__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			standard_library__float_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_result_1,
						b,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_3 = __args___x3[0];
							TinymoeObject _result_4 = __args___x3[1];
							_the_result = _result_4;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_float__COMPARE__primitive_integer_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_COMPARE__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			TinymoeObject _var_3 = null;
			_var_3 = a;
			standard_library__float_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_var_3,
						_result_1,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_4 = __args___x3[0];
							TinymoeObject _result_5 = __args___x3[1];
							_the_result = _result_5;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_string__COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_COMPARE__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			TinymoeObject _var_3 = null;
			_var_3 = a;
			standard_library__string_of__primitive(
				_state,
				b,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_var_3,
						_result_1,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_4 = __args___x3[0];
							TinymoeObject _result_5 = __args___x3[1];
							_the_result = _result_5;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_COMPARE__primitive_string_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeFunction(__args__ => standard_library__operator__expression_COMPARE__primitive(__args__[0], __args__[1], __args__[2], __args__[3]));
			standard_library__string_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_var_2, new TinymoeObject[] {
						_state_0,
						_result_1,
						b,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_3 = __args___x3[0];
							TinymoeObject _result_4 = __args___x3[1];
							_the_result = _result_4;
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__operator__expression_LT__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__operator__expression_COMPARE__primitive(
				_state,
				a,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = EQ(_result_1, Negative(new TinymoeInteger(1)));
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_GT__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__operator__expression_COMPARE__primitive(
				_state,
				a,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = EQ(_result_1, new TinymoeInteger(1));
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_LE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__operator__expression_COMPARE__primitive(
				_state,
				a,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = NE(_result_1, new TinymoeInteger(1));
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_GE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__operator__expression_COMPARE__primitive(
				_state,
				a,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = NE(_result_1, Negative(new TinymoeInteger(1)));
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_EQ__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__operator__expression_EQUAL__primitive(
				_state,
				a,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = _result_1;
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_NE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__operator__expression_EQUAL__primitive(
				_state,
				a,
				b,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Not(_result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__operator__expression_AND__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_AND_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_boolean__AND__primitive_boolean_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("b&&b")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void standard_library__operator__expression_OR__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(a.GetField("$dispatch<>standard_library::operator_$expression_OR_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void standard_library__operator__expression_boolean__OR__primitive_boolean_(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("b||b")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public class standard_library__continuation_fall_back_argument : TinymoeObject
		{
			public standard_library__continuation_fall_back_argument()
			{
				SetField("value", null);
				SetField("trap", null);
				SetField("fall_back_counter", null);
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

		public class standard_library__continuation_coroutine : TinymoeObject
		{
			public standard_library__continuation_coroutine()
			{
				SetField("body", null);
				SetField("continuation", null);
				SetField("trap", null);
				SetField("fall_back_counter", null);
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

		public void standard_library__enable_fall_back_to__expression_with__expression_and__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject continuation, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			state.SetField("continuation", continuation);
			state.SetField("argument", new standard_library__continuation_fall_back_argument().SetFields(new TinymoeObject[] {value, state.GetField("trap")}).FinishConstruction());
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__fall_back_counter_between__expression_and__expression_from__primitive(TinymoeObject _state, TinymoeObject top_trap, TinymoeObject current_trap, TinymoeObject counter, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
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
			_select_value_3 = NE(top_trap, current_trap);
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(true)))).Value)
			{
				standard_library__fall_back_counter_between__expression_and__expression_from__primitive(
					_state,
					top_trap.GetField("previous_trap"),
					current_trap,
					Add(counter, new TinymoeInteger(1)),
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						_the_result = _result_5;
						Invoke(_select_continuation_2, new TinymoeObject[] {
							_state_4,
							_result_5
							});
					})
					);
			}
			else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				_the_result = counter;
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
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

		public void hello_world__print__expression(TinymoeObject _state, TinymoeObject message, TinymoeObject _continuation)
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

		public void hello_world__sum_from__expression_to__primitive(TinymoeObject _state, TinymoeObject first_number, TinymoeObject last_number, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new TinymoeInteger(0);
			standard_library__repeat_with__argument_from__expression_to__expression(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject the_current_number = __args__[1];
					TinymoeObject _continuation_3 = __args__[2];
					standard_library__add__expression_to_assignable(
						_state_2,
						the_current_number,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _continuation_5 = __args___x2[1];
							Invoke(_continuation_5, new TinymoeObject[] {
								_state_4,
								_the_result
								});
						}),
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_6 = __args___x3[0];
							TinymoeObject _input_7 = __args___x3[1];
							TinymoeObject _continuation_8 = __args___x3[2];
							_the_result = _input_7;
							Invoke(_continuation_8, new TinymoeObject[] {
								_state_6,
								null
								});
						}),
						_continuation_3
						);
				}),
				first_number,
				last_number,
				new TinymoeFunction(__args___x4 => 
				{
					TinymoeObject _state_0 = __args___x4[0];
					TinymoeObject _result_1 = __args___x4[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void hello_world__main(TinymoeObject _state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _var_2 = null;
			_var_2 = new TinymoeString("1+ ... +10 = ");
			hello_world__sum_from__expression_to__primitive(
				_state,
				new TinymoeInteger(1),
				new TinymoeInteger(10),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					hello_world__print__expression(
						_state_0,
						Concat(_var_2, _result_1),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_3 = __args___x2[0];
							TinymoeObject _result_4 = __args___x2[1];
							standard_library__try(
								_state_3,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_7 = __args___x3[0];
									TinymoeObject _continuation_8 = __args___x3[1];
									hello_world__print__expression(
										_state_7,
										new TinymoeString("I will raise an exception."),
										new TinymoeFunction(__args___x4 => 
										{
											TinymoeObject _state_9 = __args___x4[0];
											TinymoeObject _result_10 = __args___x4[1];
											standard_library__raise__expression(
												_state_9,
												new TinymoeString("exception"),
												new TinymoeFunction(__args___x5 => 
												{
													TinymoeObject _state_11 = __args___x5[0];
													TinymoeObject _result_12 = __args___x5[1];
													hello_world__print__expression(
														_state_11,
														new TinymoeString("The world is mad!"),
														_continuation_8
														);
												})
												);
										})
										);
								}),
								new TinymoeFunction(__args___x6 => 
								{
									TinymoeObject _state_5 = __args___x6[0];
									TinymoeObject _result_6 = __args___x6[1];
									standard_library__catch__argument(
										_state_5,
										_result_6,
										new TinymoeFunction(__args___x7 => 
										{
											TinymoeObject _state_17 = __args___x7[0];
											TinymoeObject exception = __args___x7[1];
											TinymoeObject _continuation_18 = __args___x7[2];
											hello_world__print__expression(
												_state_17,
												new TinymoeString("So the exception will be caught"),
												_continuation_18
												);
										}),
										new TinymoeFunction(__args___x8 => 
										{
											TinymoeObject _state_15 = __args___x8[0];
											TinymoeObject _result_16 = __args___x8[1];
											standard_library__try(
												_state_15,
												new TinymoeFunction(__args___x9 => 
												{
													TinymoeObject _state_23 = __args___x9[0];
													TinymoeObject _continuation_24 = __args___x9[1];
													hello_world__print__expression(
														_state_23,
														new TinymoeString("I will not raise an exception."),
														new TinymoeFunction(__args___x10 => 
														{
															TinymoeObject _state_25 = __args___x10[0];
															TinymoeObject _result_26 = __args___x10[1];
															hello_world__print__expression(
																_state_25,
																new TinymoeString("So there is no exception to catch"),
																_continuation_24
																);
														})
														);
												}),
												new TinymoeFunction(__args___x11 => 
												{
													TinymoeObject _state_21 = __args___x11[0];
													TinymoeObject _result_22 = __args___x11[1];
													standard_library__catch__argument(
														_state_21,
														_result_22,
														new TinymoeFunction(__args___x12 => 
														{
															TinymoeObject _state_31 = __args___x12[0];
															TinymoeObject exception_x2 = __args___x12[1];
															TinymoeObject _continuation_32 = __args___x12[2];
															hello_world__print__expression(
																_state_31,
																new TinymoeString("The world is mad!"),
																_continuation_32
																);
														}),
														new TinymoeFunction(__args___x13 => 
														{
															TinymoeObject _state_29 = __args___x13[0];
															TinymoeObject _result_30 = __args___x13[1];
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

		public void _dispatch_fail__standard_library__number_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				Concat(Concat(new TinymoeString("number of "), value), new TinymoeString(" is illegal.")),
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

		public void TinymoeInteger___dispatch__standard_library__number_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__number_of__primitive_integer_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__number_of__primitive_x2(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__number_of__primitive_float_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeString___dispatch__standard_library__number_of__primitive_x3(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__number_of__primitive_string_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__number_of__primitive_x4(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__number_of__primitive(
				_state,
				value,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__integer_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				Concat(Concat(new TinymoeString("integer of "), value), new TinymoeString(" is illegal.")),
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

		public void TinymoeInteger___dispatch__standard_library__integer_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__integer_of__primitive_integer_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__integer_of__primitive_x2(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__integer_of__primitive_float_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeString___dispatch__standard_library__integer_of__primitive_x3(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__integer_of__primitive_string_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__integer_of__primitive_x4(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__integer_of__primitive(
				_state,
				value,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__float_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				Concat(Concat(new TinymoeString("float of "), value), new TinymoeString(" is illegal.")),
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

		public void TinymoeInteger___dispatch__standard_library__float_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__float_of__primitive_integer_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__float_of__primitive_x2(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__float_of__primitive_float_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeString___dispatch__standard_library__float_of__primitive_x3(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__float_of__primitive_string_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__float_of__primitive_x4(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__float_of__primitive(
				_state,
				value,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator_POS__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Positive(_result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator_POS__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__operator_POS__primitive_float_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator_POS__primitive_x2(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__operator_POS__primitive_integer_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator_POS__primitive_x3(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator_POS__primitive(
				_state,
				value,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator_NEG__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					_the_result = Negative(_result_1);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator_NEG__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__operator_NEG__primitive_integer_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator_NEG__primitive_x2(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__operator_NEG__primitive_float_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator_NEG__primitive_x3(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator_NEG__primitive(
				_state,
				value,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator_NOT__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				Concat(Concat(new TinymoeString("not "), value), new TinymoeString(" is illegal.")),
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

		public void TinymoeBoolean___dispatch__standard_library__operator_NOT__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			standard_library__operator_NOT__primitive_boolean_(
				_state,
				value,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator_NOT__primitive_x2(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator_NOT__primitive(
				_state,
				value,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_CONCAT__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__string_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _var_4 = null;
					_var_4 = _result_1;
					standard_library__string_of__primitive(
						_state_0,
						b,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_the_result = Concat(_var_4, _result_3);
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void TinymoeString___dispatch__standard_library__operator__expression_CONCAT__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Function>standard_library::operator_$expression_CONCAT_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeString___dispatch__Function_standard_library__operator__expression_CONCAT__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_string__CONCAT__primitive_string_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Function_standard_library__operator__expression_CONCAT__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_CONCAT__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_CONCAT__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_CONCAT__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_ADD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _var_4 = null;
					_var_4 = _result_1;
					standard_library__number_of__primitive(
						_state_0,
						b,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_the_result = Add(_var_4, _result_3);
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator__expression_ADD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_ADD_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_ADD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__ADD__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_ADD__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_ADD_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_ADD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__ADD__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_ADD__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__ADD__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_ADD__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__ADD__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_ADD__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_ADD__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_ADD__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_ADD__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_ADD__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_ADD__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_SUB__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _var_4 = null;
					_var_4 = _result_1;
					standard_library__number_of__primitive(
						_state_0,
						b,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_the_result = Sub(_var_4, _result_3);
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator__expression_SUB__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_SUB_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_SUB__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__SUB__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_SUB__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_SUB_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_SUB__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__SUB__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_SUB__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__SUB__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_SUB__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__SUB__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_SUB__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_SUB__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_SUB__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_SUB__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_SUB__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_SUB__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_MUL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _var_4 = null;
					_var_4 = _result_1;
					standard_library__number_of__primitive(
						_state_0,
						b,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_the_result = Mul(_var_4, _result_3);
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator__expression_MUL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_MUL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_MUL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__MUL__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_MUL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_MUL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_MUL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__MUL__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_MUL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__MUL__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_MUL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__MUL__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_MUL__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_MUL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_MUL__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_MUL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_MUL__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_MUL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_DIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _var_4 = null;
					_var_4 = _result_1;
					standard_library__number_of__primitive(
						_state_0,
						b,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_the_result = Div(_var_4, _result_3);
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator__expression_DIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_DIV_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_DIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__DIV__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_DIV__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_DIV_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_DIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__DIV__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_DIV__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__DIV__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_DIV__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__DIV__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_DIV__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_DIV__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_DIV__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_DIV__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_DIV__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_DIV__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_INTDIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _var_4 = null;
					_var_4 = _result_1;
					standard_library__number_of__primitive(
						_state_0,
						b,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_the_result = IntDiv(_var_4, _result_3);
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator__expression_INTDIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_INTDIV_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_INTDIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__INTDIV__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_INTDIV__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__INTDIV__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_INTDIV__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_INTDIV_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_INTDIV__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__INTDIV__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_INTDIV__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__INTDIV__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_INTDIV__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_INTDIV__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_INTDIV__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_INTDIV__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_INTDIV__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_INTDIV__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_MOD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__number_of__primitive(
				_state,
				a,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _var_4 = null;
					_var_4 = _result_1;
					standard_library__number_of__primitive(
						_state_0,
						b,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_the_result = Mod(_var_4, _result_3);
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_MOD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_MOD_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_MOD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__MOD__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__standard_library__operator__expression_MOD__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_MOD_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_MOD__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__MOD__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_MOD__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__MOD__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_MOD__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__MOD__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_MOD__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_MOD__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_MOD__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_MOD__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_MOD__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_MOD__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("o_e_o")), new TinymoeObject[] {
				_state,
				a,
				b,
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

		public void TinymoeInteger___dispatch__standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_EQUAL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__EQUAL__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_EQUAL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_EQUAL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__EQUAL__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeString___dispatch__standard_library__operator__expression_EQUAL__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Function>standard_library::operator_$expression_EQUAL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeString___dispatch__Function_standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_string__EQUAL__primitive_string_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeBoolean___dispatch__standard_library__operator__expression_EQUAL__primitive_x4(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Boolean>standard_library::operator_$expression_EQUAL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeBoolean___dispatch__Boolean_standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_boolean__EQUAL__primitive_boolean_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_EQUAL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__EQUAL__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_EQUAL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__EQUAL__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Function_standard_library__operator__expression_EQUAL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_string__EQUAL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_EQUAL__primitive_x5(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Object>standard_library::operator_$expression_EQUAL_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeString___dispatch__Object_standard_library__operator__expression_EQUAL__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_EQUAL__primitive_string_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Boolean_standard_library__operator__expression_EQUAL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_EQUAL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_EQUAL__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_EQUAL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_EQUAL__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_EQUAL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Object_standard_library__operator__expression_EQUAL__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_EQUAL__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				Concat(Concat(Concat(a, new TinymoeString(" compare ")), b), new TinymoeString(" is illegal.")),
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

		public void TinymoeInteger___dispatch__standard_library__operator__expression_COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Integer>standard_library::operator_$expression_COMPARE_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeInteger___dispatch__Integer_standard_library__operator__expression_COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__COMPARE__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__standard_library__operator__expression_COMPARE__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Float>standard_library::operator_$expression_COMPARE_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeFloat___dispatch__Float_standard_library__operator__expression_COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__COMPARE__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeString___dispatch__standard_library__operator__expression_COMPARE__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Function>standard_library::operator_$expression_COMPARE_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeString___dispatch__Function_standard_library__operator__expression_COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_string__COMPARE__primitive_string_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeFloat___dispatch__Integer_standard_library__operator__expression_COMPARE__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_integer__COMPARE__primitive_float_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeInteger___dispatch__Float_standard_library__operator__expression_COMPARE__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_float__COMPARE__primitive_integer_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Function_standard_library__operator__expression_COMPARE__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_string__COMPARE__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_COMPARE__primitive_x4(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Object>standard_library::operator_$expression_COMPARE_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeString___dispatch__Object_standard_library__operator__expression_COMPARE__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_COMPARE__primitive_string_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Float_standard_library__operator__expression_COMPARE__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_COMPARE__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Integer_standard_library__operator__expression_COMPARE__primitive_x3(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_COMPARE__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Object_standard_library__operator__expression_COMPARE__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_COMPARE__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_AND__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				Concat(Concat(Concat(a, new TinymoeString(" and ")), b), new TinymoeString("is illegal.")),
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

		public void TinymoeBoolean___dispatch__standard_library__operator__expression_AND__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Boolean>standard_library::operator_$expression_AND_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeBoolean___dispatch__Boolean_standard_library__operator__expression_AND__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_boolean__AND__primitive_boolean_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Boolean_standard_library__operator__expression_AND__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_AND__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_AND__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_AND__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void _dispatch_fail__standard_library__operator__expression_OR__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				Concat(Concat(Concat(a, new TinymoeString(" or ")), b), new TinymoeString("is illegal.")),
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

		public void TinymoeBoolean___dispatch__standard_library__operator__expression_OR__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			Invoke(b.GetField("$dispatch<$Boolean>standard_library::operator_$expression_OR_$primitive"), new TinymoeObject[] {
				_state,
				a,
				b,
				_continuation
				});
		}

		public void TinymoeBoolean___dispatch__Boolean_standard_library__operator__expression_OR__primitive(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			standard_library__operator__expression_boolean__OR__primitive_boolean_(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__Boolean_standard_library__operator__expression_OR__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_OR__primitive(
				_state,
				a,
				b,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__operator__expression_OR__primitive_x2(TinymoeObject _state, TinymoeObject a, TinymoeObject b, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__operator__expression_OR__primitive(
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
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::number_of_$primitive",
				new TinymoeFunction(__args___x3 => TinymoeInteger___dispatch__standard_library__number_of__primitive(__args___x3[0], __args___x3[1], __args___x3[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::number_of_$primitive",
				new TinymoeFunction(__args___x4 => TinymoeFloat___dispatch__standard_library__number_of__primitive_x2(__args___x4[0], __args___x4[1], __args___x4[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<>standard_library::number_of_$primitive",
				new TinymoeFunction(__args___x5 => TinymoeString___dispatch__standard_library__number_of__primitive_x3(__args___x5[0], __args___x5[1], __args___x5[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::number_of_$primitive",
				new TinymoeFunction(__args___x6 => TinymoeObject___dispatch__standard_library__number_of__primitive_x4(__args___x6[0], __args___x6[1], __args___x6[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::integer_of_$primitive",
				new TinymoeFunction(__args___x7 => TinymoeInteger___dispatch__standard_library__integer_of__primitive(__args___x7[0], __args___x7[1], __args___x7[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::integer_of_$primitive",
				new TinymoeFunction(__args___x8 => TinymoeFloat___dispatch__standard_library__integer_of__primitive_x2(__args___x8[0], __args___x8[1], __args___x8[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<>standard_library::integer_of_$primitive",
				new TinymoeFunction(__args___x9 => TinymoeString___dispatch__standard_library__integer_of__primitive_x3(__args___x9[0], __args___x9[1], __args___x9[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::integer_of_$primitive",
				new TinymoeFunction(__args___x10 => TinymoeObject___dispatch__standard_library__integer_of__primitive_x4(__args___x10[0], __args___x10[1], __args___x10[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::float_of_$primitive",
				new TinymoeFunction(__args___x11 => TinymoeInteger___dispatch__standard_library__float_of__primitive(__args___x11[0], __args___x11[1], __args___x11[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::float_of_$primitive",
				new TinymoeFunction(__args___x12 => TinymoeFloat___dispatch__standard_library__float_of__primitive_x2(__args___x12[0], __args___x12[1], __args___x12[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<>standard_library::float_of_$primitive",
				new TinymoeFunction(__args___x13 => TinymoeString___dispatch__standard_library__float_of__primitive_x3(__args___x13[0], __args___x13[1], __args___x13[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::float_of_$primitive",
				new TinymoeFunction(__args___x14 => TinymoeObject___dispatch__standard_library__float_of__primitive_x4(__args___x14[0], __args___x14[1], __args___x14[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_POS_$primitive",
				new TinymoeFunction(__args___x15 => TinymoeFloat___dispatch__standard_library__operator_POS__primitive(__args___x15[0], __args___x15[1], __args___x15[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_POS_$primitive",
				new TinymoeFunction(__args___x16 => TinymoeInteger___dispatch__standard_library__operator_POS__primitive_x2(__args___x16[0], __args___x16[1], __args___x16[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_POS_$primitive",
				new TinymoeFunction(__args___x17 => TinymoeObject___dispatch__standard_library__operator_POS__primitive_x3(__args___x17[0], __args___x17[1], __args___x17[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_NEG_$primitive",
				new TinymoeFunction(__args___x18 => TinymoeInteger___dispatch__standard_library__operator_NEG__primitive(__args___x18[0], __args___x18[1], __args___x18[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_NEG_$primitive",
				new TinymoeFunction(__args___x19 => TinymoeFloat___dispatch__standard_library__operator_NEG__primitive_x2(__args___x19[0], __args___x19[1], __args___x19[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_NEG_$primitive",
				new TinymoeFunction(__args___x20 => TinymoeObject___dispatch__standard_library__operator_NEG__primitive_x3(__args___x20[0], __args___x20[1], __args___x20[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeBoolean),
				"$dispatch<>standard_library::operator_NOT_$primitive",
				new TinymoeFunction(__args___x21 => TinymoeBoolean___dispatch__standard_library__operator_NOT__primitive(__args___x21[0], __args___x21[1], __args___x21[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_NOT_$primitive",
				new TinymoeFunction(__args___x22 => TinymoeObject___dispatch__standard_library__operator_NOT__primitive_x2(__args___x22[0], __args___x22[1], __args___x22[2]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<>standard_library::operator_$expression_CONCAT_$primitive",
				new TinymoeFunction(__args___x23 => TinymoeString___dispatch__standard_library__operator__expression_CONCAT__primitive(__args___x23[0], __args___x23[1], __args___x23[2], __args___x23[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<$Function>standard_library::operator_$expression_CONCAT_$primitive",
				new TinymoeFunction(__args___x24 => TinymoeString___dispatch__Function_standard_library__operator__expression_CONCAT__primitive(__args___x24[0], __args___x24[1], __args___x24[2], __args___x24[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Function>standard_library::operator_$expression_CONCAT_$primitive",
				new TinymoeFunction(__args___x25 => TinymoeObject___dispatch__Function_standard_library__operator__expression_CONCAT__primitive_x2(__args___x25[0], __args___x25[1], __args___x25[2], __args___x25[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_CONCAT_$primitive",
				new TinymoeFunction(__args___x26 => TinymoeObject___dispatch__standard_library__operator__expression_CONCAT__primitive_x2(__args___x26[0], __args___x26[1], __args___x26[2], __args___x26[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x27 => TinymoeInteger___dispatch__standard_library__operator__expression_ADD__primitive(__args___x27[0], __args___x27[1], __args___x27[2], __args___x27[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x28 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_ADD__primitive(__args___x28[0], __args___x28[1], __args___x28[2], __args___x28[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x29 => TinymoeFloat___dispatch__standard_library__operator__expression_ADD__primitive_x2(__args___x29[0], __args___x29[1], __args___x29[2], __args___x29[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x30 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_ADD__primitive(__args___x30[0], __args___x30[1], __args___x30[2], __args___x30[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x31 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_ADD__primitive_x2(__args___x31[0], __args___x31[1], __args___x31[2], __args___x31[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x32 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_ADD__primitive_x2(__args___x32[0], __args___x32[1], __args___x32[2], __args___x32[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x33 => TinymoeObject___dispatch__Float_standard_library__operator__expression_ADD__primitive_x3(__args___x33[0], __args___x33[1], __args___x33[2], __args___x33[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x34 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_ADD__primitive_x3(__args___x34[0], __args___x34[1], __args___x34[2], __args___x34[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_ADD_$primitive",
				new TinymoeFunction(__args___x35 => TinymoeObject___dispatch__standard_library__operator__expression_ADD__primitive_x3(__args___x35[0], __args___x35[1], __args___x35[2], __args___x35[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x36 => TinymoeInteger___dispatch__standard_library__operator__expression_SUB__primitive(__args___x36[0], __args___x36[1], __args___x36[2], __args___x36[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x37 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_SUB__primitive(__args___x37[0], __args___x37[1], __args___x37[2], __args___x37[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x38 => TinymoeFloat___dispatch__standard_library__operator__expression_SUB__primitive_x2(__args___x38[0], __args___x38[1], __args___x38[2], __args___x38[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x39 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_SUB__primitive(__args___x39[0], __args___x39[1], __args___x39[2], __args___x39[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x40 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_SUB__primitive_x2(__args___x40[0], __args___x40[1], __args___x40[2], __args___x40[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x41 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_SUB__primitive_x2(__args___x41[0], __args___x41[1], __args___x41[2], __args___x41[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x42 => TinymoeObject___dispatch__Float_standard_library__operator__expression_SUB__primitive_x3(__args___x42[0], __args___x42[1], __args___x42[2], __args___x42[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x43 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_SUB__primitive_x3(__args___x43[0], __args___x43[1], __args___x43[2], __args___x43[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_SUB_$primitive",
				new TinymoeFunction(__args___x44 => TinymoeObject___dispatch__standard_library__operator__expression_SUB__primitive_x3(__args___x44[0], __args___x44[1], __args___x44[2], __args___x44[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x45 => TinymoeInteger___dispatch__standard_library__operator__expression_MUL__primitive(__args___x45[0], __args___x45[1], __args___x45[2], __args___x45[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x46 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_MUL__primitive(__args___x46[0], __args___x46[1], __args___x46[2], __args___x46[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x47 => TinymoeFloat___dispatch__standard_library__operator__expression_MUL__primitive_x2(__args___x47[0], __args___x47[1], __args___x47[2], __args___x47[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x48 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_MUL__primitive(__args___x48[0], __args___x48[1], __args___x48[2], __args___x48[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x49 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_MUL__primitive_x2(__args___x49[0], __args___x49[1], __args___x49[2], __args___x49[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x50 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_MUL__primitive_x2(__args___x50[0], __args___x50[1], __args___x50[2], __args___x50[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x51 => TinymoeObject___dispatch__Float_standard_library__operator__expression_MUL__primitive_x3(__args___x51[0], __args___x51[1], __args___x51[2], __args___x51[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x52 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_MUL__primitive_x3(__args___x52[0], __args___x52[1], __args___x52[2], __args___x52[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_MUL_$primitive",
				new TinymoeFunction(__args___x53 => TinymoeObject___dispatch__standard_library__operator__expression_MUL__primitive_x3(__args___x53[0], __args___x53[1], __args___x53[2], __args___x53[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x54 => TinymoeInteger___dispatch__standard_library__operator__expression_DIV__primitive(__args___x54[0], __args___x54[1], __args___x54[2], __args___x54[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x55 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_DIV__primitive(__args___x55[0], __args___x55[1], __args___x55[2], __args___x55[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x56 => TinymoeFloat___dispatch__standard_library__operator__expression_DIV__primitive_x2(__args___x56[0], __args___x56[1], __args___x56[2], __args___x56[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x57 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_DIV__primitive(__args___x57[0], __args___x57[1], __args___x57[2], __args___x57[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x58 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_DIV__primitive_x2(__args___x58[0], __args___x58[1], __args___x58[2], __args___x58[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x59 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_DIV__primitive_x2(__args___x59[0], __args___x59[1], __args___x59[2], __args___x59[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x60 => TinymoeObject___dispatch__Float_standard_library__operator__expression_DIV__primitive_x3(__args___x60[0], __args___x60[1], __args___x60[2], __args___x60[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x61 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_DIV__primitive_x3(__args___x61[0], __args___x61[1], __args___x61[2], __args___x61[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_DIV_$primitive",
				new TinymoeFunction(__args___x62 => TinymoeObject___dispatch__standard_library__operator__expression_DIV__primitive_x3(__args___x62[0], __args___x62[1], __args___x62[2], __args___x62[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x63 => TinymoeInteger___dispatch__standard_library__operator__expression_INTDIV__primitive(__args___x63[0], __args___x63[1], __args___x63[2], __args___x63[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x64 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_INTDIV__primitive(__args___x64[0], __args___x64[1], __args___x64[2], __args___x64[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x65 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_INTDIV__primitive_x2(__args___x65[0], __args___x65[1], __args___x65[2], __args___x65[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x66 => TinymoeFloat___dispatch__standard_library__operator__expression_INTDIV__primitive_x2(__args___x66[0], __args___x66[1], __args___x66[2], __args___x66[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x67 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_INTDIV__primitive(__args___x67[0], __args___x67[1], __args___x67[2], __args___x67[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x68 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_INTDIV__primitive_x2(__args___x68[0], __args___x68[1], __args___x68[2], __args___x68[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x69 => TinymoeObject___dispatch__Float_standard_library__operator__expression_INTDIV__primitive_x3(__args___x69[0], __args___x69[1], __args___x69[2], __args___x69[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x70 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_INTDIV__primitive_x3(__args___x70[0], __args___x70[1], __args___x70[2], __args___x70[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_INTDIV_$primitive",
				new TinymoeFunction(__args___x71 => TinymoeObject___dispatch__standard_library__operator__expression_INTDIV__primitive_x3(__args___x71[0], __args___x71[1], __args___x71[2], __args___x71[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x72 => TinymoeFloat___dispatch__standard_library__operator__expression_MOD__primitive(__args___x72[0], __args___x72[1], __args___x72[2], __args___x72[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x73 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_MOD__primitive(__args___x73[0], __args___x73[1], __args___x73[2], __args___x73[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x74 => TinymoeInteger___dispatch__standard_library__operator__expression_MOD__primitive_x2(__args___x74[0], __args___x74[1], __args___x74[2], __args___x74[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x75 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_MOD__primitive(__args___x75[0], __args___x75[1], __args___x75[2], __args___x75[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x76 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_MOD__primitive_x2(__args___x76[0], __args___x76[1], __args___x76[2], __args___x76[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x77 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_MOD__primitive_x2(__args___x77[0], __args___x77[1], __args___x77[2], __args___x77[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x78 => TinymoeObject___dispatch__Float_standard_library__operator__expression_MOD__primitive_x3(__args___x78[0], __args___x78[1], __args___x78[2], __args___x78[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x79 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_MOD__primitive_x3(__args___x79[0], __args___x79[1], __args___x79[2], __args___x79[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_MOD_$primitive",
				new TinymoeFunction(__args___x80 => TinymoeObject___dispatch__standard_library__operator__expression_MOD__primitive_x3(__args___x80[0], __args___x80[1], __args___x80[2], __args___x80[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x81 => TinymoeInteger___dispatch__standard_library__operator__expression_EQUAL__primitive(__args___x81[0], __args___x81[1], __args___x81[2], __args___x81[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x82 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_EQUAL__primitive(__args___x82[0], __args___x82[1], __args___x82[2], __args___x82[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x83 => TinymoeFloat___dispatch__standard_library__operator__expression_EQUAL__primitive_x2(__args___x83[0], __args___x83[1], __args___x83[2], __args___x83[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x84 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_EQUAL__primitive(__args___x84[0], __args___x84[1], __args___x84[2], __args___x84[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x85 => TinymoeString___dispatch__standard_library__operator__expression_EQUAL__primitive_x3(__args___x85[0], __args___x85[1], __args___x85[2], __args___x85[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<$Function>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x86 => TinymoeString___dispatch__Function_standard_library__operator__expression_EQUAL__primitive(__args___x86[0], __args___x86[1], __args___x86[2], __args___x86[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeBoolean),
				"$dispatch<>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x87 => TinymoeBoolean___dispatch__standard_library__operator__expression_EQUAL__primitive_x4(__args___x87[0], __args___x87[1], __args___x87[2], __args___x87[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeBoolean),
				"$dispatch<$Boolean>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x88 => TinymoeBoolean___dispatch__Boolean_standard_library__operator__expression_EQUAL__primitive(__args___x88[0], __args___x88[1], __args___x88[2], __args___x88[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x89 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_EQUAL__primitive_x2(__args___x89[0], __args___x89[1], __args___x89[2], __args___x89[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x90 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_EQUAL__primitive_x2(__args___x90[0], __args___x90[1], __args___x90[2], __args___x90[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Function>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x91 => TinymoeObject___dispatch__Function_standard_library__operator__expression_EQUAL__primitive_x2(__args___x91[0], __args___x91[1], __args___x91[2], __args___x91[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x92 => TinymoeObject___dispatch__standard_library__operator__expression_EQUAL__primitive_x5(__args___x92[0], __args___x92[1], __args___x92[2], __args___x92[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<$Object>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x93 => TinymoeString___dispatch__Object_standard_library__operator__expression_EQUAL__primitive(__args___x93[0], __args___x93[1], __args___x93[2], __args___x93[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Boolean>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x94 => TinymoeObject___dispatch__Boolean_standard_library__operator__expression_EQUAL__primitive_x2(__args___x94[0], __args___x94[1], __args___x94[2], __args___x94[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x95 => TinymoeObject___dispatch__Float_standard_library__operator__expression_EQUAL__primitive_x3(__args___x95[0], __args___x95[1], __args___x95[2], __args___x95[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x96 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_EQUAL__primitive_x3(__args___x96[0], __args___x96[1], __args___x96[2], __args___x96[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Object>standard_library::operator_$expression_EQUAL_$primitive",
				new TinymoeFunction(__args___x97 => TinymoeObject___dispatch__Object_standard_library__operator__expression_EQUAL__primitive_x2(__args___x97[0], __args___x97[1], __args___x97[2], __args___x97[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x98 => TinymoeInteger___dispatch__standard_library__operator__expression_COMPARE__primitive(__args___x98[0], __args___x98[1], __args___x98[2], __args___x98[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Integer>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x99 => TinymoeInteger___dispatch__Integer_standard_library__operator__expression_COMPARE__primitive(__args___x99[0], __args___x99[1], __args___x99[2], __args___x99[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x100 => TinymoeFloat___dispatch__standard_library__operator__expression_COMPARE__primitive_x2(__args___x100[0], __args___x100[1], __args___x100[2], __args___x100[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Float>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x101 => TinymoeFloat___dispatch__Float_standard_library__operator__expression_COMPARE__primitive(__args___x101[0], __args___x101[1], __args___x101[2], __args___x101[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x102 => TinymoeString___dispatch__standard_library__operator__expression_COMPARE__primitive_x3(__args___x102[0], __args___x102[1], __args___x102[2], __args___x102[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<$Function>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x103 => TinymoeString___dispatch__Function_standard_library__operator__expression_COMPARE__primitive(__args___x103[0], __args___x103[1], __args___x103[2], __args___x103[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeFloat),
				"$dispatch<$Integer>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x104 => TinymoeFloat___dispatch__Integer_standard_library__operator__expression_COMPARE__primitive_x2(__args___x104[0], __args___x104[1], __args___x104[2], __args___x104[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeInteger),
				"$dispatch<$Float>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x105 => TinymoeInteger___dispatch__Float_standard_library__operator__expression_COMPARE__primitive_x2(__args___x105[0], __args___x105[1], __args___x105[2], __args___x105[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Function>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x106 => TinymoeObject___dispatch__Function_standard_library__operator__expression_COMPARE__primitive_x2(__args___x106[0], __args___x106[1], __args___x106[2], __args___x106[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x107 => TinymoeObject___dispatch__standard_library__operator__expression_COMPARE__primitive_x4(__args___x107[0], __args___x107[1], __args___x107[2], __args___x107[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeString),
				"$dispatch<$Object>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x108 => TinymoeString___dispatch__Object_standard_library__operator__expression_COMPARE__primitive(__args___x108[0], __args___x108[1], __args___x108[2], __args___x108[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Float>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x109 => TinymoeObject___dispatch__Float_standard_library__operator__expression_COMPARE__primitive_x3(__args___x109[0], __args___x109[1], __args___x109[2], __args___x109[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Integer>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x110 => TinymoeObject___dispatch__Integer_standard_library__operator__expression_COMPARE__primitive_x3(__args___x110[0], __args___x110[1], __args___x110[2], __args___x110[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Object>standard_library::operator_$expression_COMPARE_$primitive",
				new TinymoeFunction(__args___x111 => TinymoeObject___dispatch__Object_standard_library__operator__expression_COMPARE__primitive_x2(__args___x111[0], __args___x111[1], __args___x111[2], __args___x111[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeBoolean),
				"$dispatch<>standard_library::operator_$expression_AND_$primitive",
				new TinymoeFunction(__args___x112 => TinymoeBoolean___dispatch__standard_library__operator__expression_AND__primitive(__args___x112[0], __args___x112[1], __args___x112[2], __args___x112[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeBoolean),
				"$dispatch<$Boolean>standard_library::operator_$expression_AND_$primitive",
				new TinymoeFunction(__args___x113 => TinymoeBoolean___dispatch__Boolean_standard_library__operator__expression_AND__primitive(__args___x113[0], __args___x113[1], __args___x113[2], __args___x113[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Boolean>standard_library::operator_$expression_AND_$primitive",
				new TinymoeFunction(__args___x114 => TinymoeObject___dispatch__Boolean_standard_library__operator__expression_AND__primitive_x2(__args___x114[0], __args___x114[1], __args___x114[2], __args___x114[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_AND_$primitive",
				new TinymoeFunction(__args___x115 => TinymoeObject___dispatch__standard_library__operator__expression_AND__primitive_x2(__args___x115[0], __args___x115[1], __args___x115[2], __args___x115[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeBoolean),
				"$dispatch<>standard_library::operator_$expression_OR_$primitive",
				new TinymoeFunction(__args___x116 => TinymoeBoolean___dispatch__standard_library__operator__expression_OR__primitive(__args___x116[0], __args___x116[1], __args___x116[2], __args___x116[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeBoolean),
				"$dispatch<$Boolean>standard_library::operator_$expression_OR_$primitive",
				new TinymoeFunction(__args___x117 => TinymoeBoolean___dispatch__Boolean_standard_library__operator__expression_OR__primitive(__args___x117[0], __args___x117[1], __args___x117[2], __args___x117[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<$Boolean>standard_library::operator_$expression_OR_$primitive",
				new TinymoeFunction(__args___x118 => TinymoeObject___dispatch__Boolean_standard_library__operator__expression_OR__primitive_x2(__args___x118[0], __args___x118[1], __args___x118[2], __args___x118[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::operator_$expression_OR_$primitive",
				new TinymoeFunction(__args___x119 => TinymoeObject___dispatch__standard_library__operator__expression_OR__primitive_x2(__args___x119[0], __args___x119[1], __args___x119[2], __args___x119[3]))
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
			program.hello_world__main(state, continuation);
		}
	}
}
